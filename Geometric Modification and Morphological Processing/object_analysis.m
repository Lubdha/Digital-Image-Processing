%{
Lubdha Pimpale
4964423675
Homework 3
3rd March
%}

clear all;
close all;
h= 500;
w= 690;
file = fopen('rice.raw','r');
file_data = fread(file,w*h*3,'uint8=>uint8');

j=1;
for i=1:3:(w*h*3)-2
    R(j)=file_data(i);
    G(j)=file_data(i+1);
    B(j)=file_data(i+2);
    j=j+1;
end

red = reshape(R,w,h)';
image_rice(:,:,1)=red(:,:);

green = reshape(G,w,h)';
image_rice(:,:,2)=green(:,:);

blue = reshape(B,w,h)';
image_rice(:,:,3)=blue(:,:);

figure
imshow(image_rice)

gray_image = rgb2gray(image_rice);
figure
imshow(gray_image)

IMG = edge(gray_image,'canny', 0.1);
figure
imshow(IMG)
IMG2 = IMG;

IMG = bwmorph(IMG,'bridge');
figure
imshow(IMG)
IMG = bwmorph(IMG,'bridge');
figure
imshow(IMG)

se = strel('line',4,85);
IMG = imdilate(IMG,se);
figure
imshow(IMG);

IMG= imfill(IMG,'holes');
figure
imshow(IMG);

IMG = bwmorph(IMG,'shrink');
figure
imshow(IMG)

IMG = bwmorph(IMG,'thin',Inf);
figure
imshow(IMG)

se = strel('line',4,85);
IMG = imdilate(IMG,se);
figure
imshow(IMG);

IMG= imfill(IMG,'holes');
figure
imshow(IMG);

IMG = bwmorph(IMG,'bridge');
figure
imshow(IMG)
IMG = bwmorph(IMG,'bridge');
figure
imshow(IMG)

IMG= imfill(IMG,'holes');
figure
imshow(IMG);
IMG = bwmorph(IMG,'thin',Inf);
figure
imshow(IMG)

IMG = bwmorph(IMG,'shrink', Inf);
figure
imshow(IMG)

count=0;
for i = 1:h
    for j= 1:w
        if IMG(i,j)== 1
            count = count+ 1;
        end
    end
end

disp(count);

% this finds the size of the individual rice pieces

se1 = strel('line', 2, 90);
se2 = strel('line', 2, 0);
IMG3 = imdilate(IMG2,[se1 se2]);
IMG_fill = imfill(IMG3,'holes');
seD = strel('diamond',3);
IMG_final = imerode(IMG_fill,seD);

stats = regionprops(IMG_final,{'Area','Centroid'});
stats = struct2table(stats);

figure;
imshow(image_rice)
hold on
for kk = 1:height(stats)
    text(stats.Centroid(kk,1)+10, stats.Centroid(kk,2),...
        num2str(stats.Area(kk)))
end

% define regions
regions = [30, 40, 155, 116;
    320, 50, 410, 195;
    560, 40, 622, 122;
    30, 175, 135, 260;
    300, 175, 410, 260;
    500, 175, 680, 260;
    50, 270, 180, 380;
    300, 270, 410, 380;
    500, 270, 680, 380;
    180, 400, 290, 460;
    420, 420, 520, 480];
region_average = zeros(11, 1);

for l=1:height(stats)
    for i=1:11
        if ((stats.Centroid(l,1) > regions(i,1)) && (stats.Centroid(l,1) < regions(i,3)) && (stats.Centroid(l,2) > regions(i,2)) && (stats.Centroid(l,2) < regions(i,4)))
            region_average(i) = region_average(i) + stats.Area(l);
        end
    end
end

for i= 1:11
    region_average(i) = region_average(i)/5;
end

region_average(4) = region_average(4)*5/4;

[sorted_regions, index] = sort(region_average);
disp(index)
