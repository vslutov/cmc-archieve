function faceCollection = fatransfer_make(imageDir, facesFilename)
    if nargin < 3
        facesFilename = 'faces.mat';
    end
    
    faceCollection = Faces('make', imageDir, facesFilename);
end

