%global image set csind setlum k undo N M adj cset
k=0; cset=[]; activemin=[]; activemax=[]; [N,M]=size(image); csind=zeros(N,M);
for i=1:N
    for j=1:M
        if csind(i,j)==0
            pnbr=nbr([i j]);
            setlum=image(i,j);
            [minv,maxv]=range(image,pnbr);
            if setlum<=minv|setlum>=maxv
                k=k+1;
                set=[i j];
                csind(i,j)=k;
                adj=[];
                undo=[];
                grow(pnbr);
                for s=1:size(undo,1)
                    csind(adj(s,1),adj(s,2))=undo(s,1);
                end
                cset(k).set=set;
                cset(k).adj=adj;
                cset(k).alife=1;
                [minv,maxv]=range(image,adj);
                if minv>setlum
                    activemin=[activemin;k];
                else
                    if maxv<setlum
                        activemax=[activemax;k];
                    end
                end
            end
        end
    end
end