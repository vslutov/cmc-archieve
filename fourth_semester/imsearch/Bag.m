classdef Bag
    %BAG Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        ImagePathnames
        Dict
        KDTDict
        Histograms
        ClusterCenters
        ClusterAssign
        ReverseIndex
    end
    
    methods(Static)
        function [SIFT] = get_sift(imagePathname)
            IMG = single(rgb2gray(imread(imagePathname)));
            [~, SIFT] = vl_sift(IMG);
            SIFT = single(SIFT);
        end
    end
    
    methods
        function [obj] = Bag(method, imageDir, bagFilename)
            switch method
                case 'load'
                    bagPathname = fullfile(imageDir, bagFilename);
                    if exist(bagPathname, 'file') == 0
                        error('The bag file doesn''t exist. Please make it.');
                    else
                        load(bagPathname, 'obj');
                    end
                case 'make'
                    obj = obj.make(imageDir, bagFilename);
                otherwise
                    error(['Method should be load or make, not ', method]);
            end
        end
        
        function [nearImages] = find(obj, imagePathname, imageCount)
            clusterCount = 5;
            SIFT = Bag.get_sift(imagePathname);
            HIST = obj.get_hist(SIFT)';
            [~, sortedClusters] = sort(vl_alldist(HIST, obj.ClusterCenters));
            
            nearIndeces = cell2mat(obj.ReverseIndex(sortedClusters(1:clusterCount)));
            nearHists = obj.Histograms(:, nearIndeces);
            
            [~, nearSubindeces] = sort(vl_alldist(HIST, nearHists));
            nearImages = nearIndeces(nearSubindeces(1:imageCount));
        end
        
        function [obj] = make(obj, imageDir, bagFilename)
            imageMask = fullfile(imageDir, '*.jpg');
            imageFiles = ls(imageMask);
            wordCount = 200;
            wordClusterSize = 50;
            imageCount = size(imageFiles, 1);
            numTrees = 4; % building optimisation
            siftPerImage = 80; % precalced
            imageCountForDict = min(imageCount, ...
                ceil(wordCount * wordClusterSize / siftPerImage));
            
            siftDescriptors = cell(1, imageCountForDict);
            randomIndeces = randperm(imageCount, imageCountForDict);
            randomImageFiles = imageFiles(randomIndeces, :);
            
            parfor j = 1 : imageCountForDict
                imagePathname = fullfile(imageDir, strtrim(randomImageFiles(j, :)));
                siftDescriptors{j} = Bag.get_sift(imagePathname);
            end
            
            siftDescriptors = cell2mat(siftDescriptors);
            obj.Dict = vl_kmeans(siftDescriptors, wordCount);
            obj.KDTDict = vl_kdtreebuild(obj.Dict, 'NumTrees', numTrees);
            
            imagePathnames = cell(imageCount, 1);
            HISTOGRAMS = zeros(wordCount, imageCount, 'single');
            
            parfor i = 1 : imageCount
                iterObj = obj; % parfor hack
                imagePathnames{i} = fullfile(imageDir, strtrim(imageFiles(i, :)));
                siftDescriptor = iterObj.get_sift(imagePathnames{i});
                HISTOGRAMS(:, i) = iterObj.get_hist(siftDescriptor);
            end
            
            obj.ImagePathnames = imagePathnames;
            obj.Histograms = HISTOGRAMS;
            
            clusterCount = ceil(sqrt(imageCount));
            [obj.ClusterCenters, obj.ClusterAssign] = ...
                vl_kmeans(obj.Histograms, clusterCount);
            
            obj.ReverseIndex = cell(1, clusterCount);
            for i = 1:clusterCount
                obj.ReverseIndex{i} = find(obj.ClusterAssign == i);
            end

            save(fullfile(imageDir, bagFilename), 'obj');
        end
        
        function [HIST] = get_hist(obj, siftDescriptors)
            Words = vl_kdtreequery(obj.KDTDict, obj.Dict, siftDescriptors);
            HIST = single(histcounts(Words, size(obj.Dict, 2)));
        end
    end
end

