% Order the Giant Monster into different grips
clear all

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/NormalizedMonster.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/PredictedVector.mat')

%% G Grip
cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GiganticMatrixG.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/LabelVectorG.mat')

GripGTrainMatrix = [];
GripGTestMatrix = [];
GripGMatrix = [];
g = [];
gTrain = [];
gTest = [];

gSamples = [1:7 10:11 13:17];

for i =1:length(gSamples)
    IndexG = find(y==gSamples(i));
    GripGMatrix = [GripGMatrix; MatrixData(IndexG,:)];
   
    GripGMatrixTemp = MatrixData(IndexG,:);
    GripGMatrixRand = GripGMatrixTemp(randperm(size(GripGMatrixTemp,1)),:);
    splitValue = round(0.8*size(GripGMatrixRand,1));
    
    GripGTrainMatrix = [GripGTrainMatrix; GripGMatrixRand(1:splitValue,:)];
    GripGTestMatrix = [GripGTestMatrix; GripGMatrixRand(splitValue+1:size(GripGMatrixRand,1),:)];
    
    g = [g; y(IndexG,:)];
    gTrainTemp = y(IndexG);
    gTrain = [gTrain; gTrainTemp(1:splitValue)];
    gTestTemp = y(IndexG);
    gTest = [gTest; gTestTemp(splitValue+1:length(gTestTemp))];
    
end

save('GripGTrain', 'GripGTrainMatrix')
save('GripGTest', 'GripGTestMatrix')
save('LabelGTrain', 'gTrain')
save('LabelGTest', 'gTest')
save('GetGripG', 'GripGMatrix')
save('LabelG', 'g')


%clear all


%% M Grip
clear all

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GiganticMatrixM.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/LabelVectorM.mat')

GripMTrainMatrix = [];
GripMTestMatrix = [];
GripMMatrix = [];
m = [];
mTrain = [];
mTest = [];

mSamples = [1 3:7 10:11 13:17];

for i =1:length(mSamples)
    IndexM = find(y==mSamples(i));
    GripMMatrix = [GripMMatrix; MatrixData(IndexM,:)];
   
    GripMMatrixTemp = MatrixData(IndexM,:);
    GripMMatrixRand = GripMMatrixTemp(randperm(size(GripMMatrixTemp,1)),:);
    splitValue = round(0.8*size(GripMMatrixRand,1));
    
    GripMTrainMatrix = [GripMTrainMatrix; GripMMatrixRand(1:splitValue,:)];
    GripMTestMatrix = [GripMTestMatrix; GripMMatrixRand(splitValue+1:size(GripMMatrixRand,1),:)];
    
    m = [m; y(IndexM,:)];
    mTrainTemp = y(IndexM);
    mTrain = [mTrain; mTrainTemp(1:splitValue)];
    mTestTemp = y(IndexM);
    mTest = [mTest; mTestTemp(splitValue+1:length(mTestTemp))];
    
end

save('GripMTrain', 'GripMTrainMatrix')
save('GripMTest', 'GripMTestMatrix')
save('LabelMTrain', 'mTrain')
save('LabelMTest', 'mTest')
save('GetGripM', 'GripMMatrix')
save('LabelM', 'm')


%clear all


%% P Grip

GripPTrainMatrix = [];
GripPTestMatrix = [];
GripPMatrix = [];
p = [];
pTrain = [];
pTest = [];

pSamples = [3 7 9 16 18 24 29 43 47];

for i =1:length(pSamples)
    IndexP = find(y==pSamples(i));
    GripPMatrix = [GripPMatrix; NormalizedMatrixMonster(IndexP,:)];
   
    GripPMatrixTemp = NormalizedMatrixMonster(IndexP,:);
    GripPMatrixRand = GripPMatrixTemp(randperm(size(GripPMatrixTemp,1)),:);
    splitValue = round(0.8*size(GripPMatrixRand,1));
    
    GripPTrainMatrix = [GripPTrainMatrix; GripPMatrixRand(1:splitValue,:)];
    GripPTestMatrix = [GripPTestMatrix; GripPMatrixRand(splitValue+1:size(GripPMatrixRand,1),:)];
    
    p = [p; y(IndexP,:)];
    pTrainTemp = y(IndexP);
    pTrain = [pTrain; pTrainTemp(1:splitValue)];
    pTestTemp = y(IndexP);
    pTest = [pTest; pTestTemp(splitValue+1:length(pTestTemp))];
    
end

save('GripPTrain', 'GripPTrainMatrix')
save('GripPTest', 'GripPTestMatrix')
save('PredictedPTrain', 'pTrain')
save('PredictedPTest', 'pTest')
save('GetGripP', 'GripPMatrix')
save('PredictedP', 'p')

% clear all

%% W Grip

clear all

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GiganticMatrixW.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/LabelVectorW.mat')

GripWTrainMatrix = [];
GripWTestMatrix = [];
GripWMatrix = [];
w = [];
wTrain = [];
wTest = [];

wSamples = [1:7 10 11 13 16 17];

for i =1:length(wSamples)
    IndexW = find(y==wSamples(i));
    GripWMatrix = [GripWMatrix; MatrixData(IndexW,:)];
   
    GripWMatrixTemp = MatrixData(IndexW,:);
    GripWMatrixRand = GripWMatrixTemp(randperm(size(GripWMatrixTemp,1)),:);
    splitValue = round(0.8*size(GripWMatrixRand,1));
    
    GripWTrainMatrix = [GripWTrainMatrix; GripWMatrixRand(1:splitValue,:)];
    GripWTestMatrix = [GripWTestMatrix; GripWMatrixRand(splitValue+1:size(GripWMatrixRand,1),:)];
    
    w = [w; y(IndexW,:)];
    wTrainTemp = y(IndexW);
    wTrain = [wTrain; wTrainTemp(1:splitValue)];
    wTestTemp = y(IndexW);
    wTest = [wTest; wTestTemp(splitValue+1:length(wTestTemp))];
    
end

save('GripWTrain', 'GripWTrainMatrix')
save('GripWTest', 'GripWTestMatrix')
save('LabelWTrain', 'wTrain')
save('LabelWTest', 'wTest')
save('GetGripW', 'GripWMatrix')
save('LabelW', 'w')

% clear all

%%
GripTTrainMatrix = [];
GripTTestMatrix = [];
GripTMatrix = [];
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
    
    GripTTrainMatrix = [GripTTrainMatrix; GripTMatrixRand(1:splitValue,:)];
    GripTTestMatrix = [GripTTestMatrix; GripTMatrixRand(splitValue+1:size(GripTMatrixRand,1),:)];
    
    t = [t; y(IndexT,:)];
    tTrainTemp = y(IndexT);
    tTrain = [tTrain; tTrainTemp(1:splitValue)];
    tTestTemp = y(IndexT);
    tTest = [tTest; tTestTemp(splitValue+1:length(tTestTemp))];
    
end

save('GripTTrain', 'GripTTrainMatrix')
save('GripTTest', 'GripTTestMatrix')
save('PredictedTTrain', 'tTrain')
save('PredictedTTest', 'tTest')
save('GetGripT', 'GripTMatrix')
save('PredictedT', 't')

%clear all


%%

clear all
cd ..
cd ..