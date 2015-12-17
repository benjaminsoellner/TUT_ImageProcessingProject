% Task 2: load the images
function imgBinary = load_image(imgFile, imgTreshold)
    imgBinary = rgb2gray(imread(imgFile));  % load
    imgBinary = 1-uint8(ceil(imgBinary / imgTreshold) > 1); % treshold
    imgBinary = imdilate(imgBinary, [0,1,1,1,0; 1,1,1,1,1; 1,1,1,1,1; 1,1,1,1,1; 0,1,1,1,0]); % dilate
end
