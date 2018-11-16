function [edgeFilters] = edge_filters(rotateCount, sigmaCount)
    global leanStorage % lean execution
    if size(leanStorage, 1) == 0
        leanStorage = {};
    end
    if size(leanStorage, 1) >= rotateCount && ...
       size(leanStorage, 2) >= sigmaCount && ...
       ~isempty(leanStorage{rotateCount, sigmaCount})
   
        edgeFilters = leanStorage{rotateCount, sigmaCount};
        return
    end

    
    edgeFilters = cell(sigmaCount * rotateCount, 1);
    for SIGMA = 1 : sigmaCount
        filterSize = 6 * SIGMA + 1;
        for ROTATE = 1 : rotateCount
            edgeFilters{(SIGMA - 1) * rotateCount + ROTATE} = ...
                edge_filter(ROTATE / rotateCount * pi, SIGMA, filterSize);
        end
    end
    leanStorage{rotateCount, sigmaCount} = edgeFilters;
end

function [edgeFilter] = edge_filter(ROTATE, SIGMA, filterSize)
    halfSize = floor(filterSize / 2);
    [X, Y] = meshgrid((-halfSize:halfSize));
    
    d = 0.01; % differecial
    dX = d * sin(ROTATE) * SIGMA;
    dY = d * cos(ROTATE) * SIGMA;
    dT = d * SIGMA;
    
    negVal = gaussmf(sqrt((X - dX).^2 + (Y - dY).^2), SIGMA);
    posVal = gaussmf(sqrt((X + dX).^2 + (Y + dY).^2), SIGMA);
    edgeFilter = (posVal - negVal) / (2 * dT);
    edgeFilter = edgeFilter / sum(abs(edgeFilter(:)));
end

function [N] = gaussmf(A, SIGMA)
    N = exp(- A.^2 / (2 * SIGMA^2));
end