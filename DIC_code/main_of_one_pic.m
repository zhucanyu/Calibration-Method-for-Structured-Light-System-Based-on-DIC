clc
close all
clear all

% it is a DIC code to find matching coordinates of one 'centers*.csv' in one
% '*.bmp'in 'speckle.bmp' and save them as 'centersnew*.csv' ,you can use
% it to test if the code work
% ��������Ҫ��ȡҪƥ�������ͼƬ�����м���

files = dir(fullfile('board_pics\','*.bmp'));
filesd = dir(fullfile('speckle_pic\','*.bmp'));
lengthFiles = length(files);

r=30;%���������뾶 (subsets radius)
search_radius = 200;%���������뾶 (search radius)
count = 1 ;%����ͼƬ��� ��Number of the image being calculated��

ei  = 1;bi=1;
err=nan;bug=nan;
filename   =   strcat('centers',num2str(count),'.csv');
image_reference = uint8(imread(strcat('board_pics\',files(2*count-1).name)));%�ļ�����·��

figure(count);
imshow(image_reference);
title(strcat('ԭʼͼ��',num2str(count)));
hold on
%����ο�ͼ��ʶ�����Բ������������Ϊ�������ĵ�
centers=csvread(filename);

POI_point_count = size(centers,1); 
x_pos1(:)=NaN;
y_pos1(:)=NaN;

for i=1:POI_point_count      %�������������λ��
            x_pos1(i) =centers(i,1);
            y_pos1(i) =centers(i,2);
           plot(x_pos1,y_pos1,'*b');       
end
hold off

 
filename   =   strcat('centers',num2str(count),'.csv');

image_reference = uint8(imread(strcat('board_pics\',files(2*count-1).name)));%�ļ�����·��
image_deformed = uint8(imread(strcat('speckle_pic\',filesd(1).name)));
figure(count);
hold on
[height, width]=size(image_reference);%ȡ��ͼƬ��С

centers=csvread(filename); %����ο�ͼ��ʶ�����Բ������������Ϊ�������ĵ�
POI_point_count = size(centers,1); 
x_pos1(POI_point_count)=NaN;
y_pos1(POI_point_count)=NaN;

for i=1:POI_point_count      %����������ı��
            x_pos1(i) =centers(i,1);
            y_pos1(i) =centers(i,2);          
end
for j=1:POI_point_count
text(centers(j,1),centers(j,2),num2str(j),'color','y');
end

figure(count+100);
imshow(image_deformed);
title('��׼ͼ��');
hold on

result_matx(POI_point_count)=NaN;
result_maty(POI_point_count)=NaN;

x_pos(POI_point_count)=NaN;
y_pos(POI_point_count)=NaN;
    for i=1:POI_point_count   %ʹ�òο�ֵȡλ��������λ�ã������ڿ�ʼ�����ֵ���
     x_pos(i)=fix(x_pos1(i));
     y_pos(i)=fix(y_pos1(i));
    end

    
   for i=1:POI_point_count
         refer_subset = zeros(2*r+1,2*r+1);
         deformed_subset = zeros(2*r+1 ,2*r+1);
          normb=10;   
            % �ο�ģ��ĻҶȾ���
            for m = y_pos(i) -r : y_pos(i) +r
                for n = x_pos(i) -r : x_pos(i) +r
                    patch_m = m - y_pos(i) + r + 1;
                    patch_n = n - x_pos(i) + r + 1;
                    gray_level = image_reference(m,n);
                    refer_subset(patch_m , patch_n) =gray_level;
                end
            end
            m=NaN;
            n=NaN;
            % ������Χ�ڲ�����ƥ��
            correlation_coefficient_mat_per_point = zeros(2*search_radius+1,2*search_radius+1); % �洢������Χ�ڵ����ֵ���󣬲��Ƚ�������С�ģ���Ϊ������ƥ���
            for re_deformed_subset_center_y = y_pos(i) - search_radius : y_pos(i) + search_radius % ������Χ
                 for re_deformed_subset_center_x= x_pos(i) -search_radius : x_pos(i) + search_radius 
                 % �Ըõ�Ϊ�е㣬�����������,ֻ��ƽ��
                    for m = re_deformed_subset_center_y - r : re_deformed_subset_center_y + r  
                         for n = re_deformed_subset_center_x - r : re_deformed_subset_center_x +r
                                patch_m = m - re_deformed_subset_center_y +r + 1 ;
                                patch_n = n - re_deformed_subset_center_x +r + 1 ;
                                gray_level = image_deformed(m,n);
                                deformed_subset(patch_m,patch_n) = gray_level; 
                            
                         end
                     end    
                        
                     correlation_coefficient = Calc_correlation_coefficient(refer_subset,deformed_subset);%���ϵ������
                      
                     index_k = re_deformed_subset_center_y - y_pos(i) + search_radius + 1;
                     index_l = re_deformed_subset_center_x - x_pos(i) + search_radius + 1;
                        
                     correlation_coefficient_mat_per_point(index_k,index_l) = correlation_coefficient;
                    
                 end
            end 
               
         
            [y,x]=find(correlation_coefficient_mat_per_point==min(min(correlation_coefficient_mat_per_point)));
          
            resultx=x_pos(i)-search_radius+x-1;
            resulty=y_pos(i)-search_radius+y-1;
            result_matx(i)=resultx;
            result_maty(i)=resulty;
            
            %�����Ͻ����Ϊsub-pixel����ĳ�ֵ���ƣ���ic_gn�����������ؼ���
            U= resultx-x_pos(i);
            V= resulty-y_pos(i);
            
            sub_u_result_matx(i)=U;
            sub_v_result_maty(i)=V;
            
            %IC_GN
            P=[U,0,0,V,0,0].'; %�����κ���������ֵ
            middle_mat=Middle_mat(refer_subset,r); %����ο������Ÿ��Ⱦ���
 
            for IT_count=1:20
                interpolation_deform_subset=Interpolation_deform(image_deformed,P,x_pos(i),y_pos(i),r);%��ֵ����Ŀ�������Ҷ�
                [delta_P,P_next]=IC_GN2(middle_mat,refer_subset,interpolation_deform_subset,r,P);
                P=P_next;
                norm=sqrt(delta_P(1)^2+(delta_P(2)*r)^2+(delta_P(3)*r)^2+delta_P(4)^2+(delta_P(5)*r)^2+(delta_P(6)*r)^2);%P�ķ���
                fprintf('%d , %d, %f\n',i,IT_count,norm);
               if norm<=0.01                    
                   break
               end
               
               if ((P(1)-U)^2+(P(4)-V)^2)>100    
                   bug(bi)=i;
                   bi=bi+1;
                   break
                end
               %�жϣ�ƫ�������ص�̫��;
            end
            
            if (IT_count>400)
                err(ei)=i;
                ei=ei+1;
            end
            sub_u_result_matx(i)=P(1);
            sub_v_result_maty(i)=P(4);
            
            %����������λ��
    
            x_posnew(i) =x_pos1(i)+sub_u_result_matx(i);
            y_posnew(i) =y_pos1(i)+sub_v_result_maty(i);
            plot(x_posnew(i),y_posnew(i),'o','Color','y');
            plot(x_posnew(i),y_posnew(i),'.','Color','r');
            hold on
            
    end   



    
    %{
for i=1:POI_point_count      %����������λ��
    
            x_posnew(i) =x_pos1(i)+sub_u_result_matx(i);
            y_posnew(i) =y_pos1(i)+sub_v_result_maty(i);
            plot(x_posnew(i),y_posnew(i),'o','Color','y');
            hold on
    end
    for j=1:POI_point_count 
            text(x_posnew(j),y_posnew(j),num2str(j));
    end
    %}
centersnew=zeros(POI_point_count,2)*NaN;
centersnew(:,1)=x_posnew(:)';
centersnew(:,2)=y_posnew(:)';
filename1   =   strcat('centersnew',num2str(count),'.csv');
csvwrite(filename1, centersnew);
filename1   =   strcat('centersnew',num2str(count),'.mat');
save(filename1, 'centersnew');
