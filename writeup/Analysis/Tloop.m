GripTMatrix = [];
GripTTrain = [];
GripTTest = [];
t = [];
tTrain = [];
tTest = [];

tSamples = [4 15 21 26 28 34 36 39 41 44 49];

for i =1:length(tSamples)
    IndexT = find(y==tSamples(i));
    GripTMatrix = [GripTMatrix; NormalizedMatrixMonster(IndexT,:)];
   
    GripTMatrixTemp = NormalizedMatrixMonster(IndexT,:);
    GripTMatrixRand = GripTMatrixTemp(randperm(size(GripTMatrixTemp,1)),:);
    splitValue = round(0.8*size(GripTMatrixRand,1));
    
    GripTTrain = [GripTTrain; GripTMatrixRand(1:splitValue,:)];
    GripTTest = [GripTTest; GripTMatrixRand(splitValue+1:size(GripTMatrixTemp,1),:)];
    
    t = [t; y(IndexT,:)];
    tTrain = [tTrain; y(1:splitValue,:)];
    tTest = [tTest; y(splitValue+1:length(IndexT),:)];
    
end
