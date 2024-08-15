library("rio")
library("igraph")
set.seed(1)
x <-import("in.txt");
g<-graph_from_data_frame(x, directed = TRUE, vertices = NULL);
SCC <- clusters(g, mode="strong")
V(g)$color <- rainbow(SCC$no)[SCC$membership]
plot(g, mark.groups = split(1:vcount(g), SCC$membership))
