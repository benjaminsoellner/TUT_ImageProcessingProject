% Task 7: generate prototype vectors
function prototypeVectors = generate_prototype_vectors(imgFiles, imgTresholds, L, T)
    prototypeVectors = cell(1, length(imgFiles));
    % for every image
    for image = 1:length(imgFiles)
        % load the image and label the objects
        imgFile = imgFiles{image};
        imgTreshold = imgTresholds{image}
        imgData = load_image(imgFile, imgTreshold);
        [imgObjects, numObjects] = label_image(imgData, T);
        % for every object
        prototypeVector = [];
        for object = 1:numObjects
            % isolate the object and trace its boundaries
            objBinary   = (imgObjects == object);
            objBoundary = trace_boundary(objBinary);
            % if the boundary lengh is not zero, generate the pattern vector from it.
            % and add the pattern vector to the prototype vector
            if (length(objBoundary) ~= 0) 
                objPattern = generate_pattern_vector(objBoundary, L);
                prototypeVector = [prototypeVector; objPattern];
            end
        end
        % add the prototype vector to the output-cell-array
        prototypeVectors{image} = prototypeVector;
    end
end