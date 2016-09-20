function y=nbr(p)
global N
global M
k=0;
i=p(1,1);j=p(1,2);
if i>1
    k=k+1;
    y(k,1)=i-1;y(k,2)=j;
end
if j>1
    k=k+1;
    y(k,1)=i;y(k,2)=j-1;
end;
if i<N
    k=k+1;
    y(k,1)=i+1;y(k,2)=j;
end
if j<M
    k=k+1;
    y(k,1)=i;y(k,2)=j+1;
end