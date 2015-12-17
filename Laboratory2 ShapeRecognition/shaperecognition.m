% Joonas Meriläinen 176182 joonas.merilainen@tut.fi
% Benjamin Söllner 206342 benjamin.sollner@tut.fi (Lintula: sollner)
%
% image files can be found on harakka in
% /home/sollner/pub/

% Put here the image folder
chdir 'P:\Eigene Dateien\(S) Studien\[SF4] Finnland 4\[SGN-3056] Digital Image Processing 2\Laboratory2 ShapeRecognition\';

% Filenames
imgTrainingFiles{1} = 'img1';
imgTrainingFiles{2} = 'img2';
imgTrainingFiles{3} = 'img3';
imgTestFile = 'img4';
% Tresholds
imgTrainingTresholds{1} = 100;
imgTrainingTresholds{2} = 100;
imgTrainingTresholds{3} = 100;
imgTestTreshold = 100;


L = 10; % Pattern vector accuracy
T = 500; % Object size treshold
S = 0.05; % smoother for boundary (mean filter with window size of factor S times the boundary length)

classify_objects(imgTrainingFiles, imgTrainingTresholds, imgTestFile, imgTestTreshold, L, T);
classify_objects_alternative(imgTestFile, imgTestTreshold, T, S);