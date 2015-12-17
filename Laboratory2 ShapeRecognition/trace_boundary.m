% Task 4: boundary tracing
function pv = trace_boundary(imgBinary);
    imgSize = size(imgBinary);
    pointvector = []; pv = [];
    % n ... number of boundery points already found         1
    % d ... direction where last step came from           2 + 0
    %       according to image displayed on the right       3
    % 1.) search from top left until a pixel with
    %     pixel value 1 is found
    n = 0; % we need it later
    finished = 0;
    for y = 1:imgSize(1)
        for x = 1: imgSize(2)
            if imgBinary(y,x) == 1
                % add this pixel to the boundary vector
                currentY = y;
                currentX = x;
                pointvector = [pointvector; currentY,currentX];
                d = 3; % we came from above looking down
                n = 1; % size of pointvector now one
                % 2.) as long as we did not "make the round" around
                %     the boundary
                while (n <= 2) | (pointvector(n, 1) ~= pointvector(1, 1)) | (pointvector(n, 2) ~= pointvector(1, 2))
                    % 3.) search the 3x3 neighbourhood of the current pixel
                    %     for the first point with value 1
                    %     starting "right" from where we came from mod(d+3,4)
                    %     and doing one round
                    neighbourfound = 0;
                    for dd = [mod(d+3,4), mod(d,4), mod(d+1,4), mod(d+2,4)]
                        % find coordinates of current neighbourhood pixel
                        switch dd
                            case 0 % neighbour is on the right of current pixel
                                neighbourY = currentY; neighbourX = currentX+1;
                            case 1 % neighbour is above current pixel
                                neighbourY = currentY-1; neighbourX = currentX;
                            case 2 % neighbour is on the left of current pixel %%OK
                                neighbourY = currentY; neighbourX = currentX-1;
                            case 3 % neighbour is below current pixel
                                neighbourY = currentY+1; neighbourX = currentX;
                        end
                        % test, if this pixel is a boundary pixel
                        if (imgBinary(neighbourY, neighbourX) == 1)
                            % this is our new current pixel
                            currentY = neighbourY;
                            currentX = neighbourX;
                            d = dd;
                            % add this pixel to the boundary vector
                            pointvector = [pointvector; currentY,currentX];
                            n = n+1;
                            % we can now stop searching the neighbourhood and
                            % proceed with our next pixel (go to (2.)) ...
                            neighbourfound = 1;
                            break
                        end
                        % ... but if this was no boundary pixel, we have to
                        % continue searching the neighbourhood (go to (3.))
                    end % of for dd = [mod(d+3,4), ...
                    if neighbourfound == 0
                        [currentY, currentX, dd, d]
                        error 'Found a pixel without neigbour pixels.'
                    end
                end % of while (n <= 2) | ...
                % 4.) Ok, we should have the boundary now, no need to finish
                % iterating through the picture (1.).
                finished = 1;
                break
            end % of if imgBinary(y,x) == 1
        end % of for x = 1: imgSize(2)
        % Took me 2 hours to get the insight that for (4.) we also have to 
        % break out of the outer-most for-function. :-(
        if finished
            break
        end
    end % of for y = 1:imgSize(1)
    
    % convert to complex numbers, remove last element
    for k = 1:n-1
        pv = [pv, pointvector(k,2)-i*pointvector(k,1)];
    end
end