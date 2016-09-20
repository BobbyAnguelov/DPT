function [minv,maxv]=range(image,set)
v=[];
for i=1:size(set,1)
    v(i)=image(set(i,1),set(i,2));
end
minv=min(v);
maxv=max(v);