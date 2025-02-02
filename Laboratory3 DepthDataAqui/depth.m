%Joonas Meril�inen 176182 joonas.merilainen@tut.fi
%Benjamin S�llner 206342 benjamin.sollner@tut.fi
%Depth Data Acquisition Using the Defocusing Method

%==== DRT algorithm ====
%Laplacian band-pass filter
laplacian_f = [-2 -1 -2;-1 12 -1;-2 -1 -2];
%Gaussian 3x3 low-pass filter with standard deviation of 0.85
gaussian_f = fspecial('gaussian', [3, 3], 0.85);
%Read two images to variables G1 and grayscale2 in greyscale
grayscale1 = rgb2gray(imread('PICT0035.TIF'));
grayscale2 = rgb2gray(imread('PICT0038.TIF'));

%Images are normalized before filtering
grayscale1 = (grayscale1-min(min(grayscale1)))/(max(max(grayscale1))-min(min(grayscale1)));
grayscale2 = (grayscale2-min(min(grayscale2)))/(max(max(grayscale2))-min(min(grayscale2)));

%Images are filtered with gaussian filter
grayscale1 = filter2(gaussian_f, grayscale1);
grayscale2 = filter2(gaussian_f, grayscale2);

%Images are bandbassed with laplacian filters
grayscale1 = filter2(laplacian_f, grayscale1);
grayscale2 = filter2(laplacian_f, grayscale2);

%Pixel values are squared to get laplacian energies
L1 = grayscale1.^2;
L2 = grayscale2.^2;

%Energies are summed inside an area to get average powers
h1 = 1/8*size(L1,2);
h2 = 1/8*size(L2,2);
w1 = 1/8*size(L1,2);
w2 = 1/8*size(L2,2);
powers1 = filter2( ones(h1, w1), L1 );
powers2 = filter2( ones(h2, w2), L2 );

%Relative power difference
power_difference = 2*(powers2-powers1)./(powers2+powers1);
%"Dummy" blurred image
P1g = filter2(gaussian_f, grayscale1);
P1g = filter2(laplacian_f, P1g);
%Relative power difference between original image and P1g
power_difference2 = 2*(P1g-powers1)./(powers1+P1g);
C = power_difference./power_difference2;

%Cutting away edges of resulting matrix
C = C(h1:size(C,1)-2, h1:size(C,2)-2);

%Smoothen the coefficient image C
AVG = fspecial('average', 3);
C = filter2(AVG, C);

%Compute distances
K = 8000;
f = 42.9;
q = 48;
s = 1/(1/f-1/q);
sc1 = size(C,1);
sc2 = size(C,2);
distances = ones(size(C));
for verind = 1:sc1
    for horind = 1:sc2  
        X = roots([K, -K/s, -C(verind,horind)]);
        if (X(1)~=0)
            distances(verind, horind) = -f/(-f*X(1)-1);
        else
            distances(verind, horind) = -f/(-f*X(2)-1);
        end
    end
end

figure(123);
colormap(flipud(cool)); % use reverse "cool" colormap
newsurface=surf(distances);
get(newsurface)
%[displays the alternatives for adjusting the parameters]
%get(axes) %[displays information about the axes]
set(gca,'Zdir','reverse')
set(newsurface,'Facecolor','interp' ,'Facelighting','none', ...
'Linestyle','none')
zlabel('cm');
title('3D shape of the object');
%figure(123); set(gca,'Zdir','reverse')
%[activates the figure123 and changes the z-axis data direction]
zdata=get(newsurface,'Zdata'); %[obtains data from the plot]
%Real distances are in matrix named distances

%Distances difference
difference = real(max(max(distances))-min(min(distances)));

