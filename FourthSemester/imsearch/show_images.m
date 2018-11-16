function show_images(imagePathnames, IMAGES)
    imageCount = size(IMAGES, 2);
    sqrtCount = ceil(sqrt(imageCount));
    figure('Position', get(groot,'ScreenSize'), 'Name', 'Finded images');
    for i = 1 : imageCount
        subplot(sqrtCount, sqrtCount, i);
        imagePathname = imagePathnames{IMAGES(i)};
        subimage(imread(imagePathname));
    end
end

