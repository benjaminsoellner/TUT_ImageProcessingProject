% Task 6: display pattern vectors    
function display_pattern_vectors(imgFile, imgTreshold, L, T, fig)
    imgData = load_image(imgFile, imgTreshold);
    [imgObjects, numObjects] = label_image(imgData, T);
    output = [];
    for object = 1:numObjects
        objBinary   = (imgObjects == object);
        objBoundary = trace_boundary(objBinary);
        if (length(objBoundary) ~= 0) 
            objPattern  = generate_pattern_vector(objBoundary, L);
            output = [output; objPattern];
        end
    end
    figure(fig);
    subplot(1,2,1), imshow(imgObjects);
    subplot(1,2,2), stem(-L+2:L-1, output');
end