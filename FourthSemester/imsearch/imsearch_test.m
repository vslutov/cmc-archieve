function [gistPoints, bagPoints] = imsearch_test(imageDir, gistFilename, bagFilename)
    if nargin < 3
        bagFilename = 'bag.mat';
    end
    
    if nargin < 2
        gistFilename = 'gist.mat';
    end
    
    groupSize = 10; % How many similar images we should search
    testCount = 5000; % How many images we should test
    maxSuccess = groupSize * testCount;

    GIST = Gist('load', imageDir, gistFilename);
    BAG = Bag('load', imageDir, bagFilename);
    
    IMAGES = get_image_table(GIST.ImagePathnames);
    randomSuccess = random_success(IMAGES, groupSize, testCount);
    gistSuccess = method_success(GIST, IMAGES, groupSize, testCount);
    gistPoints = gistSuccess / randomSuccess;
    disp(char('Gist:', ...
              ['  Success: ' num2str(gistSuccess) '/' num2str(maxSuccess) ...
               ' (' num2str(gistSuccess / maxSuccess * 100) '%)'], ...
              ['  Score: ' num2str(gistPoints)]));
    bagSuccess = method_success(BAG, IMAGES, groupSize, testCount);
    bagPoints = bagSuccess / randomSuccess;
    disp(char('Bag:', ...
              ['  Success: ' num2str(bagSuccess) '/' num2str(maxSuccess) ...
               ' (' num2str(bagSuccess / maxSuccess * 100) '%)'], ...
              ['  Score: ' num2str(bagPoints)]));
end

function [IMAGES] = get_image_table(imagePathnames)
    CATEGORY = cell(size(imagePathnames, 1), 1);

    for i = 1:size(imagePathnames, 1)
        [~, name, ~] = fileparts(imagePathnames{i});
        UNDERSCORES = strfind(name, '_');
        CATEGORY{i} = name(1:UNDERSCORES(end) - 1);
    end
    
    PATHNAME = imagePathnames;
    PATHNAME = cell2table(PATHNAME);
    CATEGORY = cell2table(CATEGORY);
    CATEGORY.CATEGORY = categorical(CATEGORY.CATEGORY);
    IMAGES = [PATHNAME, CATEGORY];
end

function [randomSuccess] = random_success(IMAGES, groupSize, testCount)
    CATS = categories(IMAGES.CATEGORY);
    catSize = zeros(size(CATS, 1), 1);
    for i = 1:size(CATS, 1)
        catSize(i) = sum(IMAGES.CATEGORY == CATS(i));        
    end
    randomSuccess = sum(catSize(:) .^ 2) / size(IMAGES, 1)^2 * testCount * groupSize;
end

function [methodSuccess] = method_success(METHOD, IMAGES, groupSize, testCount)
    methodSuccess = zeros(testCount, 1);
    randomIndeces = randperm(size(IMAGES, 1), testCount);
    randomPathnames = IMAGES.PATHNAME(randomIndeces);
    randomCategories = METHOD.ReverseIndex(METHOD.ClusterAssign(randomIndeces));
    parfor i = 1:testCount
        ITER_METHOD = METHOD; % broadcast hack
        nearImages = ITER_METHOD.find(randomPathnames{i}, groupSize);
        methodSuccess(i) = size(intersect(randomCategories{i}, nearImages), 2);
    end
    methodSuccess = sum(methodSuccess);
end
