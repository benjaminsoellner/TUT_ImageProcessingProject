% Task 9: classify objects alternatively
function classify_objects_alternative(imgTestFile, imgTestTreshold, T, smoother)    
    tolerance = 5;
    imgTestData = load_image(imgTestFile, imgTestTreshold);
    [imgObjects, numObjects] = label_image(imgTestData, T);
    
    % classify all the objects
    assignedClasses = [];
    for object = 1:numObjects
        % isolate the object and trace its boundaries
        objBinary   = (imgObjects == object);
        % get boundaries
        objBoundary = trace_boundary(objBinary);
        if length(objBoundary) ~= 0
            smooth = length(objBoundary)*smoother;
            X = -imag(objBoundary);
            Y =  real(objBoundary);
            % smooth the boundaries (do padding before)
            X = [X(1)*ones(1,smooth/2), X, X(length(X))*ones(1,smooth/2)];
            Y = [Y(1)*ones(1,smooth/2), Y, Y(length(Y))*ones(1,smooth/2)];
            X = filter2(ones(1,smooth)/smooth, X, 'valid');
            Y = filter2(ones(1,smooth)/smooth, Y, 'valid');
            % a corner is present, where the first derivates of either the X- and the Y-coordinate (or both) of the the
            % boundary vector is changing 
            % 1. compute the maximum change inside a sliding window for each point of the derivative
            cornersX = nlfilter(diff(X), [1 ceil(smooth)], inline('max(abs(S-mean(S)))'));
            cornersY = nlfilter(diff(Y), [1 ceil(smooth)], inline('max(abs(S-mean(S)))'));
            corners = 0;
            % 2. traverse this function and look for regions with value above average
            inCornerRegion = 0;
            counter = 0;
            for i = 1:length(cornersX)
                if (cornersX(i) > mean(cornersX) | cornersY(i) > mean(cornersY))
                    if inCornerRegion == 0
                        corners = corners+1;
                        inCornerRegion = 1;
                        counter = tolerance;
                    end
                else
                    if counter == 0
                        inCornerRegion = 0;
                    else
                        counter = counter-1;
                    end
                end
            end
            % if corner detected at start and at end, substract 1 from corners
            if ( (cornersX(1)                > mean(cornersX) | cornersY(1)                > mean(cornersY)) & ...
                 (cornersX(length(cornersX)) > mean(cornersX) | cornersY(length(cornersY)) > mean(cornersY)) )
                corners = corners-1;
            end
            if corners >= 3 & corners <= 5
                assignedClasses = [assignedClasses; corners-2];
            else
                assignedClasses = [assignedClasses; 0];
            end
        else % of if length(objBoundary) ~= 0
            assignedClasses = [assignedClasses; 0];
        end
    end
    
    % now, re-label the test image according to assigned classes
    [height, width] = size(imgObjects);
    for y = 1:height
        for x = 1:width
            if imgObjects(y,x) ~= 0
                imgObjects(y,x) = assignedClasses(imgObjects(y,x));
            end
        end
    end
    % display image
    figure(2), imshow(imgObjects/max(assignedClasses)); colormap('jet'), colorbar;
    
end