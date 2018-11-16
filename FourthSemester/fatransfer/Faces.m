classdef Faces
    %FACES Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        Models
        Option
        FaceProps
        Descriptors
        ImagePathnames
        
        ClusterCenters
        ClusterAssign
        ReverseIndex
    end
    
    properties(Constant)
        POINT_COUNT = 49;
        TEST_FACE = struct('prev', single([]), ...
                           'pred', single([]), ...
                           'desc', single([]));
    end
    
    methods
        function obj = Faces(method, imageDir, facesFilename)
            switch method
                case 'load'
                    gistPathname = fullfile(imageDir, facesFilename);
                    if exist(gistPathname, 'file') == 0
                        error('The faces file doesn''t exist. Please make it.');
                    else
                        load(gistPathname, 'obj');
                        [obj.Models, obj.Option] = xx_initialize;
                    end
                case 'make'
                    [obj.Models, obj.Option] = xx_initialize;
                    obj = obj.make(imageDir, facesFilename);
                otherwise
                    error(['Method should be load or make, not ', method]);
            end
        end
        
        function FACE = get_face(obj, IMG)
            FACE = Faces.TEST_FACE;
            findedFaces = obj.Models.DM{1}.fd_h.detect(IMG, ...
                'MinNeighbors', obj.Option.min_neighbors,...
                'ScaleFactor', 1.2, ...
                'MinSize', [50, 50]);
            for i = 1:length(findedFaces)
                oneFace = xx_track_detect(obj.Models, IMG, findedFaces{i}, obj.Option);
                if ~isempty(oneFace.pred)
                    FACE.pred = single(oneFace.pred);
                    FACE.prev = [min(FACE.pred(:, 1)), min(FACE.pred(:, 2)), max(FACE.pred(:, 1)), max(FACE.pred(:, 2))];
                    FACE.prev(3:4) = FACE.prev(3:4) - FACE.prev(1:2);
                    FACE.desc(1 : Faces.POINT_COUNT) = ...
                        (FACE.pred(:, 1) - FACE.prev(1)) ./ FACE.prev(3);
                    FACE.desc(Faces.POINT_COUNT + 1 : 2 * Faces.POINT_COUNT) = ...
                        (FACE.pred(:, 2) - FACE.prev(2)) ./ FACE.prev(4);
                    FACE.desc = FACE.desc';
                    FACE.prev = int32(FACE.prev);
                    return;
                end
            end
        end
        
        function [imgFace, imgProps] = load_similar(obj, faceDescriptor)
            [~, clusterNum] = min(vl_alldist(faceDescriptor, obj.ClusterCenters));
            CLUSTER = obj.ReverseIndex{clusterNum};
            DESCRIPTORS = obj.Descriptors(:, CLUSTER);
            [~, imgIndex] =  min(vl_alldist(faceDescriptor, DESCRIPTORS));
            imgIndex = CLUSTER(imgIndex);
            imgPathname = obj.ImagePathnames{imgIndex};
            imgFace = imread(imgPathname);
            imgProps = obj.get_face(imgFace);
        end
        
        function obj = make(obj, imageDir, facesFilename)
            imageMask = fullfile(imageDir, '*.jpg');
            imageFiles = ls(imageMask);
            imageCount = size(imageFiles, 1);
            imagePathnames = cell(imageCount, 1);
            faceProps = Faces.TEST_FACE;
            faceDescriptors = zeros(2 * Faces.POINT_COUNT, imageCount, 'single');
            
            goodImages = true(1, imageCount);
            for i = 1 : imageCount
                parforObj = obj;
                imageFilename = strtrim(imageFiles(i, :));
                imagePathnames{i} = fullfile(imageDir, imageFilename);
                IMG = imread(imagePathnames{i});
                faceProps(i) = parforObj.get_face(IMG);
                if ~isempty(faceProps(i).desc)
                    faceDescriptors(:, i) = faceProps(i).desc;
                else
                    goodImages(i) = false;
                end
            end
            
            obj.Descriptors = faceDescriptors(:, goodImages);
            obj.FaceProps = faceProps(goodImages);
            obj.ImagePathnames = imagePathnames(goodImages);
            
            clusterCount = ceil(sqrt(size(obj.Descriptors, 2)));
            [obj.ClusterCenters, obj.ClusterAssign] = ...
                vl_kmeans(obj.Descriptors, clusterCount);
            
            obj.ReverseIndex = cell(1, clusterCount);
            for i = 1:clusterCount
                obj.ReverseIndex{i} = find(obj.ClusterAssign == i);
            end
            
            save(fullfile(imageDir, facesFilename), 'obj');
        end
    end
end

