function reconstruct(szmin,szmax)
global ddp dup image
kmin=1;
while size(ddp(kmin).set,1)<szmin
    kmin=kmin+1;
end
kmax=size(ddp,2);
while size(ddp(kmax).set,1)>szmax
    kmax=kmax-1;
end
for k=kmin:kmax
     for s=1:size(ddp(k).set,1)
         image(ddp(k).set(s,1),ddp(k).set(s,2))=image(ddp(k).set(s,1),ddp(k).set(s,2))+ddp(k).rellum;
     end
end
kmin=1;
while size(dup(kmin).set,1)<szmin
    kmin=kmin+1;
end
kmax=size(dup,2);
while size(dup(kmax).set,1)>szmax
    kmax=kmax-1;
end
for k=kmin:kmax
     for s=1:size(dup(k).set,1)
         image(dup(k).set(s,1),dup(k).set(s,2))=image(dup(k).set(s,1),dup(k).set(s,2))+dup(k).rellum;
     end
 end