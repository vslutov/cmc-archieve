classdef Gist
    %GIST Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        ImagePathnames
        Descriptors
        ClusterCenters
        ClusterAssign
        ReverseIndex
    end
    
   	methods (Static)
        function [gistDescriptor] = calc(IMG)
            cellSize = [128, 128];
            cellCount = 4;
            colorCount = 3;
            rotateCount = 6;
            sigmaCount = 5;
            filterCount = rotateCount * sigmaCount;
            channelCount = colorCount + filterCount;
            
            imageChannels = cell(channelCount, 1);
            rgbImage = imresize(im2single(IMG), cellSize * cellCount);
            if size(rgbImage, 3) == 1 % If image has colors
                grayImage = rgbImage;
                rgbImage = cat(3, rgbImage, rgbImage, rgbImage);
            else
                grayImage = rgb2gray(rgbImage);
            end
            
            for i = 1 : colorCount
                imageChannels{i} = squeeze(rgbImage(:, :, i));
            end
            
            edgeFilters = edge_filters(rotateCount, sigmaCount);
            for i = 1 : filterCount
                imageChannels{colorCount + i} = imfilter(grayImage, edgeFilters{i});
            end
            
            blockCount = cellCount^2;
            channelBlocks = cell(channelCount, blockCount);
            for i = 1 : channelCount
                cellChannel = mat2cell(imageChannels{i}, ...
                    repmat(cellSize(1), 1, cellCount), ...
                    repmat(cellSize(2), 1, cellCount));
                channelBlocks(i, :) = cellChannel(:);
            end
            channelBlocks = channelBlocks(:);
            
            gistDescriptor = zeros(size(channelBlocks, 1), 1, 'single');
            for i = 1 : size(channelBlocks, 1)
                gistDescriptor(i) = mean2(channelBlocks{i});
            end
            
            for SIGMA = 1:sigmaCount
                sigmaLayer = gistDescriptor(colorCount + (SIGMA - 1) * rotateCount + 1 : ...
                                            colorCount + SIGMA * rotateCount);
                gistDescriptor(colorCount + (SIGMA - 1) * rotateCount + 1 : ...
                               colorCount + SIGMA * rotateCount) = sigmaLayer ./ sum(abs(sigmaLayer));
            end
        end
    end
    
    methods
       function obj = Gist(method, imageDir, gistFilename)
            switch method
                case 'load'
                    gistPathname = fullfile(imageDir, gistFilename);
                    if exist(gistPathname, 'file') == 0
                        error('The gist file doesn''t exist. Please make it.');
                    else
                        load(gistPathname, 'obj');
                    end
                case 'make'
                    obj = obj.make(imageDir, gistFilename);
                otherwise
                    error(['Method should be load or make, not ', method]);
            end
        end
        
        function [nearImages] = find(obj, imagePathname, imageCount)
            clusterCount = 5;
            IMG = imread(imagePathname);
            gistDescriptor = Gist.calc(IMG);
            [~, sortedClusters] = sort(vl_alldist(gistDescriptor, obj.ClusterCenters));
            
            nearIndeces = cell2mat(obj.ReverseIndex(sortedClusters(1:clusterCount)));
            nearGists = obj.Descriptors(:, nearIndeces);
            
            [~, nearSubindeces] = sort(vl_alldist(gistDescriptor, nearGists));
            nearImages = nearIndeces(nearSubindeces(1:imageCount));
        end
        
        function obj = make(obj, imageDir, gistFilename)
            imageMask = fullfile(imageDir, '*.jpg');
            imageFiles = ls(imageMask);
            imageCount = size(imageFiles, 1);
            
            testImage = cat(3, 0, 0, 0);
            gistSize = size(Gist.calc(testImage), 1);
            gistDescriptors = zeros(gistSize, imageCount, 'single');
            imagePathnames = cell(imageCount, 1);
            
            parfor i = 1 : imageCount
                imageFilename = strtrim(imageFiles(i, :));
                imagePathnames{i} = fullfile(imageDir, imageFilename);
                IMG = imread(imagePathnames{i});
                gistDescriptors(:, i) = Gist.calc(IMG);
            end
            
            obj.Descriptors = gistDescriptors;
            obj.ImagePathnames = imagePathnames;
            
            clusterCount = ceil(sqrt(size(gistDescriptors, 2)));
            [obj.ClusterCenters, obj.ClusterAssign] = ...
                vl_kmeans(gistDescriptors, clusterCount);
            
            obj.ReverseIndex = cell(1, clusterCount);
            for i = 1:clusterCount
                obj.ReverseIndex{i} = find(obj.ClusterAssign == i);
            end
            
            save(fullfile(imageDir, gistFilename), 'obj');
        end
    end
end

