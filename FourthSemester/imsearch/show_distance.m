function show_distance(X)
    Distance = vl_alldist2(X);
    maxDistance = max(Distance(:));
    for i = 1:size(Distance, 1)
        Distance(i, i) = maxDistance;
    end
    Distance = min(Distance);
    histogram(Distance(:));
end

