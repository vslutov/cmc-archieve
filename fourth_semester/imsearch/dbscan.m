function [ClusterAssign] = dbscan(D, minPts)
    C = 1;
    EPS = eps(D, minPts);
    pointCount = size(D, 2);
    Visited = false(pointCount, 1);
    for i = 1:cointCount
        if visited(i)
            continue
        end
        visited(i) = true;
        neighborPts = regionQuery(i, D, EPS);
        if size(neighborPts, 1) < minPts
            ClusterAssign(i) = 1;
        else
            C = C + 1;
            ClusterAssign = expand_cluster(ClusterAssign, i, neighborPTS, C, D, EPS, minPts);
        end
    end
end

function [ClusterAssign] = expand_cluster(ClusterAssign, i, neighborPTS, C, D, EPS, minPts)
    ClusterAssign(i) = C;
    for 
end

expandCluster(P, NeighborPts, C, eps, MinPts) {
   add P to cluster C
   for each point P' in NeighborPts { 
      if P' is not visited {
         mark P' as visited
         NeighborPts' = regionQuery(P', eps)
         if sizeof(NeighborPts') >= MinPts
            NeighborPts = NeighborPts joined with NeighborPts'
      }
      if P' is not yet member of any cluster
         add P' to cluster C
   }
}

regionQuery(P, eps)
   return all points within P's eps-neighborhood (including P)