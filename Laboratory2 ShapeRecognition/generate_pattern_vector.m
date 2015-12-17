% Task 5: compute pattern vector from fourier descriptors
function p = generate_pattern_vector(boundary, L)
    % make fourier descriptors [C0, C1, C2, ..., CN-1]
    fd = fft(boundary);
    % get left side of pattern vector nominator [|CN-L|, |CN-L-1|, ..., |CN-1|]
    pln = abs( fd(length(fd)-L+1:length(fd)) );
    % get right side of pattern vector nominator [|C2|, |C3|, |C4|, ..., |CL|]
    prn = abs( fd(3:L) );
    % get pattern vector denominator |C1|
    pd  = abs( fd(2) );
    % make pattern vector (glue left and right parts together, divide by denominator)
    p = [pln/pd, prn/pd];
end
