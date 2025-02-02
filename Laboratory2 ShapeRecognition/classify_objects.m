% Task 8: classify objects
function classify_objects(imgTrainingFiles, imgTrainingTresholds, imgTestFile, imgTestTreshold, L, T)
    
    % generate training set prototype vectors
    trainingPrototypes = generate_prototype_vectors(imgTrainingFiles, imgTrainingTresholds, L, T);
    % generate test set prototype vectors   
    imgTestFiles = cell(1,1);
    imgTestFiles{1} = imgTestFile;
    imgTestTresholds = cell(1,1);
    imgTestTresholds{1} = imgTestTreshold;
    testPrototypes = generate_prototype_vectors(imgTestFiles, imgTestTresholds, L, T);
    testPrototype = testPrototypes{1};
    % here we will save which class all the objects are assigned to
    assignedClasses = [];
    
    % for every test object
    [testObjectsLength, testVectorLength] = size(testPrototype);
    for testObjectIndex = 1:testObjectsLength
        % get the test prototype vector
        testVector = testPrototype(testObjectIndex, :);
        % compare it to every training class ..
        classDistances = [];
        for trainingPrototypeIndex = 1:length(trainingPrototypes)
            trainingPrototype = trainingPrototypes{trainingPrototypeIndex};
            % .. and for every training class, compare it with every object
            [trainingObjectsLength, trainingVectorLength] = size(trainingPrototype);
            distances = [];
            for trainingObjectIndex = 1:trainingObjectsLength
                % first get the training prototype vector
                trainingVector = trainingPrototype(trainingObjectIndex, :);
                % compute the eukledian distance (nearest neighbour classification)
                distance = sqrt(sum((testVector-trainingVector).^2));
                distances = [distances; distance];
            end % of for trainingObjectIndex = 1:trainingObjectsLength
            % save the minimal distance of all objects of the tested class
            classDistances = [classDistances; min(distances)];
        end % of for trainingPrototypeIndex = 1:length(trainingPrototypes)
        % find the minimal distance and the corresponding class
        assignedClass = 0;
        if length(classDistances) > 0
            assignedClass = 1;
            minimumFound = classDistances(1);
            for classIndex = 2:length(classDistances)
                if classDistances(classIndex) < minimumFound
                    assignedClass = classIndex;
                    minimumFound = classDistances(classIndex);
                end
            end
        end
        assignedClasses = [assignedClasses; assignedClass];
    end % of for testObjectIndex = 1:testObjectsLength
    
    % now, re-label the test image according to assigned classes
    imgTestData = load_image(imgTestFile, imgTestTreshold);
    [imgTestObjects, numTestObjects] = label_image(imgTestData, T);
    [height, width] = size(imgTestObjects);
    for y = 1:height
        for x = 1:width
            if imgTestObjects(y,x) ~= 0
                imgTestObjects(y,x) = assignedClasses(imgTestObjects(y,x));
            end
        end
    end
    
    % display image
    figure(1), imshow(imgTestObjects/3), colormap('jet'), colorbar;

end
