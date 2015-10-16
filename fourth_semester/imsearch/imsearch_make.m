function [GIST, BAG] = imsearch_make(imageDir, gistFilename, bagFilename)
    if nargin < 3
        bagFilename = 'bag.mat';
    end
    
    if nargin < 2
        gistFilename = 'gist.mat';
    end
    
    GIST = Gist('make', imageDir, gistFilename);
    BAG = Bag('make', imageDir, bagFilename);
end

