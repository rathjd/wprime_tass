import os
from GetLocalFileNames import GetSampleTypes

SampleYears = ["2016apv","2016","2017","2018"]
# SampleTypes = ["SingleElectron","SingleMuon"]
# SampleTypes.extend(["ttbar"])
# SampleTypes.extend(["wjets_HT_70_100", "wjets_HT_100_200", "wjets_HT_200_400", "wjets_HT_400_600","wjets_HT_600_800", "wjets_HT_800_1200", "wjets_HT_1200_2500", "wjets_HT_2500_inf"])
# SampleTypes.extend(["single_antitop_tchan","single_antitop_tw","single_top_schan","single_top_tchan","single_top_tw"])
# SampleTypes.extend(["Private_FL_M500"])
SampleTypes = GetSampleTypes()
print(SampleTypes)

EOSPath = "/eos/cms/store/group/phys_b2g/wprime/"
PersonalEOSPath = "/eos/cms/store/user/r/rathjd/"
EOSSubFolderName = "SifuFW_Fitted/"

AuxHistsPath = EOSPath + "AuxHistsSifu/"
if not os.path.exists(AuxHistsPath):
  os.makedirs(AuxHistsPath)
AuxHistsPathBatch = AuxHistsPath + "batch/"
if not os.path.exists(AuxHistsPath + "batch/"):
  os.makedirs(AuxHistsPathBatch)

with open("Utilities/UserSpecificsSkeleton.cc","r") as skeleton:
  NewFile = "Utilities/UserSpecifics.cc"
  fout = open(NewFile,"w")
  # EOSLines = skeleton.read()
  for el in skeleton:
    el = el.replace("Replacement_EOSBasePath",EOSPath)
    fout.write(el)

if not os.path.exists("Submits"):
  os.makedirs("Submits")


RefreshSubmit = True
with open("Submits/SubmitTemplate.sh","r") as skeleton:
  if RefreshSubmit:
    NewFile = "Submits/Submit.sh"
    fout = open(NewFile,"w")
    for el in skeleton:
      el = el.replace("__cwd__",os.getcwd())
      fout.write(el)

#FIXME: filelist splitting is now that files have been cut down to manageable sizes to be purged
BatchHistKeywords = ["SingleMuon", "SingleElectron", "ttbar"] # Keyworks for samples that contain too many events in the output tree, that making hist out of them take too long, to run batch jobs.
for iy, year in enumerate(SampleYears):
  if not iy==2: continue #FIXME: only 2017
  for isa, sampletype in enumerate(SampleTypes):
    print(SampleYears[iy] + "| " + str(isa) + ":" + sampletype)
    filenamesfolder = "filenames/"
    filenamesfile = (sampletype + "_" + year + ".txt")
    runname = year + "_" + sampletype
    fnfile = filenamesfolder + filenamesfile
    if not os.path.exists(fnfile): continue

    nf = len(open(fnfile).readlines())
    if nf == 0: continue
    fpj = 1.0 
    lines = []
    # lines.append("Proxy_path   = /afs/cern.ch/user/s/siluo/x509up\n")
    # lines.append("arguments    = $(Proxy_path) $(ProcID) "+str(iy)+ " " + str(isa) + "\n")
    lines.append("arguments    = $(ProcID) "+str(iy)+ " " + str(isa) + " 0\n")
    lines.append("executable   = Submit.sh\n")
    lines.append("max_retries  = 10\n")
    # lines.append("+JobBatchName= " + runname +"\n")
    lines.append("batch_name   = " + runname +"\n")
    lines.append("output       = logs/"+runname+"/"+runname+"_$(ProcID).out\n")
    lines.append("error        = logs/"+runname+"/"+runname+"_$(ProcID).err\n")
    lines.append("log          = logs/"+runname+"/"+runname+"_$(ProcID).log\n")
    lines.append("universe     = vanilla\n")
    lines.append('Requirements = (OpSysAndVer =?= "AlmaLinux9")\n')
    # flavours: espresso 20min, microcentury 1h, longlunch 2h, workday 8h, tomorrow 1d, testmatch 3d, nextweek 1w
    lines.append('+JobFlavour  = "nextweek"\n')
    lines.append("RequestCpus  = 2\n")
    lines.append("stream_error = True\n")
    lines.append("periodic_release =  (NumJobStarts < 10) && ((CurrentTime - EnteredCurrentStatus) > (5*60))\n")
    lines.append("queue "+str(nf)+"\n")

    fn = "Submits/" + runname + ".sub"
    f = open(fn, "w")
    f.writelines(lines)

    logpath = "Submits/logs/" + runname
    if not os.path.exists("Submits/logs/" + runname):
      os.makedirs("Submits/logs/" + runname)

    jobsubfolder = year + "_" + sampletype + "/"
    jobpath = EOSPath + EOSSubFolderName + jobsubfolder
    if not os.path.exists(jobpath):
      os.makedirs(jobpath)
