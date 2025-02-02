% Task 3: label the regions
function [imgLabeled, numObjects] = label_image(imgBinary, T);
    imgSize = size(imgBinary);
    [imgLabeled, numObjects] = bwlabel(imgBinary, 4);
    labelsCounter = zeros(numObjects, 1);
    % Run through image and count objects or look,
    % if they are on the boundary
    for y = 1:imgSize(1)
        for x = 1:imgSize(2)
            if (imgLabeled(y,x) ~= 0)
                if (y == 1) | (y == imgSize(1)) | (x == 1) | (x == imgSize(2)) | (labelsCounter(imgLabeled(y,x)) == -1)
                    % if object touches the frame or touched the frame once, discard the object
                    labelsCounter(imgLabeled(y,x)) = -1;
                else 
                    % otherwise increase area counter
                    labelsCounter(imgLabeled(y,x)) = labelsCounter(imgLabeled(y,x)) + 1;
                end
            end
        end
    end
    % Create a mapping table
    labelsMapper = zeros(numObjects, 1);
    k = 1;
    for o = 1:numObjects
        if (labelsCounter(o) < T)
            labelsMapper(o) = 0;
        else
            labelsMapper(o) = k;
            k = k+1;
        end
    end
    % update labeled image and number of objects
    numObjects = k;
    for y = 1:imgSize(1)
        for x = 1:imgSize(2)
            if imgLabeled(y,x) ~= 0
                imgLabeled(y,x) = labelsMapper(imgLabeled(y,x));
            end
        end
    end
end
