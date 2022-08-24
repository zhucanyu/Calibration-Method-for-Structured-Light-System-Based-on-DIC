function [gra_x,gra_y] = gradient2(refer_subset)
S=size(refer_subset);
r=(S(1)-1)/2;
%���ݶȾ���
gra_x=zeros(2*r+1,2*r+1);
gra_y=zeros(2*r+1,2*r+1);
%������x�����ݶȾ���
for m=1:2*r+1
    row=refer_subset(m,:);
    gra=gradient(row,2*r+1);
    gra_x(m,:)=gra;
end
%������y�����ݶȾ���
for n=1:2*r+1
    col=refer_subset(:,n);
    gra=gradient(col,2*r+1);
    gra_y(:,n)=gra;
end
end

