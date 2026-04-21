import os,sys

#macro to generate separate workspaces and toys for all signal injection test scenarios

#signal strengths at 50% median expected fully blinded limits at 95% CL in 100 GeV signal mass steps from 300 to 1100 GeV
SignalStrength = ["0.04", "0.14", "0.21", "0.32", "0.46", "0.63", "0.90", "1.20", "1.67"]

#get the job number and convert into which operation to run
try:
    if sys.argv[1]:
        jobID = int(sys.argv[1])
        print("jobID set to", jobID)
except:
    print("no jobID set, aborting with error code 1")
    sys.exit(1)

#make number of toys variable
nToys = "500"
try:
    if sys.argv[2]:
        nToys = sys.argv[2]
        print("nToys set to", nToys)
except:
    print("nToys defaults to", nToys)

#set minimal and maximal signal strength
rMin = "-40"
try:
    if sys.argv[3]:
        rMin = sys.argv[3]
        print("rMin set to", rMin)
except:
    print("rMin defaults to", rMin)

rMax = "10"
try:
    if sys.argv[4]:
        rMax = sys.argv[4]
        print("rMax set to", rMax)
except:
    print("rMax defaults to", rMax)


#reform jobID into mass and mode
mode   = jobID // 9 #0 is no signal injection, 1 is signal injection at expected strength
massID = jobID % 9
mass   = str((massID+3)*100)

print("running mode",mode,"at mass",mass)

#set the appropriate signal strength
SignalR = "0.00"
if mode == 1:
    SignalR = SignalStrength[massID]

#generate workspace with channel masking enabled
if os.path.isfile("workspace_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".root"):
    os.system("rm workspace_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".root")
os.system("text2workspace.py Combination/Optimized_WprimeAll_all_M"+mass+".txt --channel-masks -o workspace_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".root")

#make a snapshot to get postfit values for CR data only
if os.path.isfile("higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.root"):
    os.system("rm higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.root")
os.system("combine -M MultiDimFit -d workspace_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".root --algo fixed --fixedPointPOIs r=0 -n _SignalInjectionTest_R"+SignalR+"_MWp"+mass+" --saveWorkspace --setParameters mask_ch1_ch10=1,mask_ch2_ch10=1,mask_ch3_ch10=1,mask_ch4_ch10=1,mask_ch1_ch3_ch1=1,mask_ch2_ch3_ch1=1,mask_ch3_ch3_ch1=1,mask_ch4_ch3_ch1=1,mask_ch1_ch3_ch2=1,mask_ch2_ch3_ch2=1,mask_ch3_ch3_ch2=1,mask_ch4_ch3_ch2=1,mask_ch1_ch4_ch1=1,mask_ch2_ch4_ch1=1,mask_ch3_ch4_ch1=1,mask_ch4_ch4_ch1=1,mask_ch1_ch4_ch2=1,mask_ch2_ch4_ch2=1,mask_ch3_ch4_ch2=1,mask_ch4_ch4_ch2=1,mask_ch1_ch7_ch1=1,mask_ch2_ch7_ch1=1,mask_ch3_ch7_ch1=1,mask_ch4_ch7_ch1=1,mask_ch1_ch7_ch2=1,mask_ch2_ch7_ch2=1,mask_ch3_ch7_ch2=1,mask_ch4_ch7_ch2=1,mask_ch1_ch8_ch1=1,mask_ch2_ch8_ch1=1,mask_ch3_ch8_ch1=1,mask_ch4_ch8_ch1=1,mask_ch1_ch8_ch2=1,mask_ch2_ch8_ch2=1,mask_ch3_ch8_ch2=1,mask_ch4_ch8_ch2=1,mask_ch1_ch9=1,mask_ch2_ch9=1,mask_ch3_ch9=1,mask_ch4_ch9=1")

#generate toys from snapshot with signal at the expected limit
if os.path.isfile("higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".GenerateOnly.mH120.123456.root"):
    os.system("rm higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".GenerateOnly.mH120.123456.root")
os.system("combine -M GenerateOnly -d higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.root --snapshotName MultiDimFit --expectSignal "+SignalR+" --saveToys --toysFrequentist --bypassFrequentistFit -t "+nToys+" -n _SignalInjectionTest_R"+SignalR+"_MWp"+mass+" --rMax "+rMax)

#fit with toy data in signal regions with signal, control regions masked
if os.path.isfile("higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.123456.root"):
    os.system("rm higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.123456.root")
os.system("combine -M MultiDimFit workspace_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".root -t "+nToys+" -n _SignalInjectionTest_R"+SignalR+"_MWp"+mass+" --toysFile higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".GenerateOnly.mH120.123456.root --algo singles --toysFrequentist --rMin "+rMin+" --rMax "+rMax+" --setParameters mask_ch1_ch1_ch1=1,mask_ch2_ch1_ch1=1,mask_ch3_ch1_ch1=1,mask_ch4_ch1_ch1=1,mask_ch1_ch1_ch2=1,mask_ch2_ch1_ch2=1,mask_ch3_ch1_ch2=1,mask_ch4_ch1_ch2=1,mask_ch1_ch2_ch1=1,mask_ch2_ch2_ch1=1,mask_ch3_ch2_ch1=1,mask_ch4_ch2_ch1=1,mask_ch1_ch2_ch2=1,mask_ch2_ch2_ch2=1,mask_ch3_ch2_ch2=1,mask_ch4_ch2_ch2=1,mask_ch1_ch5_ch1=1,mask_ch2_ch5_ch1=1,mask_ch3_ch5_ch1=1,mask_ch4_ch5_ch1=1,mask_ch1_ch5_ch2=1,mask_ch2_ch5_ch2=1,mask_ch3_ch5_ch2=1,mask_ch4_ch5_ch2=1,mask_ch1_ch6_ch1=1,mask_ch2_ch6_ch1=1,mask_ch3_ch6_ch1=1,mask_ch4_ch6_ch1=1,mask_ch1_ch6_ch2=1,mask_ch2_ch6_ch2=1,mask_ch3_ch6_ch2=1,mask_ch4_ch6_ch2=1")

#make corresponding plot
if os.path.isfile("pull_r"+SignalR+"_MWp"+mass+".png"):
    os.system("rm pull_r"+SignalR+"_MWp"+mass+".png")
os.system("python3 InjectionTestPlotter.py --r_truth="+SignalR+" --r_max="+rMax+" --r_min="+rMin+" --file higgsCombine_SignalInjectionTest_R"+SignalR+"_MWp"+mass+".MultiDimFit.mH120.123456.root")
os.rename("pull_r"+SignalR+".png", "pull_r"+SignalR+"_MWp"+mass+".png")

