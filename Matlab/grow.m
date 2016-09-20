function grow(new)
global image setlum k %input
global set undo adj %cumulative output
global cset csind %input and update
for s=1:size(new,1)
    switch csind(new(s,1),new(s,2))
        case 0
            if image(new(s,1),new(s,2))==setlum;
                set=[set;new(s,:)];
                csind(new(s,1),new(s,2))=k;
                grow(nbr(new(s,:)));
            else
                adj=[adj;new(s,:)];
                csind(new(s,1),new(s,2))=k;
                undo=[undo;0];
            end
        case k
        otherwise
            kk=csind(new(s,1),new(s,2));
            if image(new(s,1),new(s,2))==setlum;
                set=[set;cset(kk).set];
                for s=1:size(cset(kk).set,1)
                    csind(cset(kk).set(s,1),cset(kk).set(s,2))=k;
                end
                cset(kk).alife=0;
                grow(cset(kk).adj);
            else
                adj=[adj;new(s,:)];
                csind(new(s,1),new(s,2))=k;
                undo=[undo;kk];
            end
        end
    end
end