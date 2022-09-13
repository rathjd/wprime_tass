import os

SampleYears = ["2016apv","2016","2017","2018"]
SampleTypes = ["SingleElectron","SingleMuon","ttbar","wjets_HT_70_100", "wjets_HT_100_200", "wjets_HT_200_400", "wjets_HT_400_600","wjets_HT_600_800", "wjets_HT_800_1200", "wjets_HT_1200_2500", "wjets_HT_2500_inf","single_antitop_tchan","single_antitop_tw","single_top_schan","single_top_tchan","single_top_tw"]
Triggers = ["SE","SM"]

RunningYears = ["2016","2017"]
RunningTypes = ["ttbar"]
RunningTriggers = ["SE"]

EOSFolderName = "PUEval"

if not os.path.exists("Submits"):
  os.makedirs("Submits")

MCBaseFolder = "/eos/user/d/doverton/"
for iy, year in enumerate(SampleYears):
  if not (year in RunningYears): continue
  for isa, sampletype in enumerate(SampleTypes):
    if not (sampletype in RunningTypes): continue
    for itr, trigger in enumerate(Triggers):
      if not (trigger in RunningTriggers): continue
      # filenamesfolder = "/afs/cern.ch/work/s/siluo/wprime/datafiles/" if (sampletype == "SingleElectron" or sampletype == "SingleMuon") else "/afs/cern.ch/work/s/siluo/wprime/mcfiles/"
      if isa + itr == 1: continue
      filenamesfolder = "/afs/cern.ch/work/s/siluo/wprime/filenames/"
      filenamesfile = (sampletype + "_" + year + ".txt")
      runname = year + "_" + sampletype + "_" +trigger
      fnfile = filenamesfolder + filenamesfile
      if not os.path.exists(fnfile): continue
      if (sampletype == "SingleElectron" and trigger == "SM") or (sampletype == "SingleMuon" and trigger == "SE"): continue

      nf = len(open(fnfile).readlines())
      lines = []
      lines.append("Proxy_path   = /afs/cern.ch/user/s/siluo/x509up\n")
      lines.append("arguments    = $(Proxy_path) $(ProcID) "+str(iy)+ " " + str(isa) + " " + str(itr) + "\n")
      lines.append("executable   = Submit.sh\n")
      lines.append("max_retries  = 10\n")
      # lines.append("+JobBatchName= " + runname +"\n")
      lines.append("batch_name   = " + runname +"\n")
      lines.append("output       = logs/"+runname+"/$(ClusterID)_$(ProcID).out\n")
      lines.append("error        = logs/"+runname+"/$(ClusterID)_$(ProcID).err\n")
      lines.append("log          = logs/"+runname+"/$(ClusterID)_$(ProcID).log\n")
      lines.append("universe     = vanilla\n")
      lines.append('Requirements = (OpSysAndVer =?= "CentOS7")\n')
      lines.append('+JobFlavour  = "workday"\n')
      lines.append("RequestCpus  = 2\n")
      lines.append("periodic_release =  (NumJobStarts < 10) && ((CurrentTime - EnteredCurrentStatus) > (5*60))\n")
      lines.append("queue "+str(nf)+"\n")

      fn = "Submits/" + runname + ".sub"
      f = open(fn, "w")
      f.writelines(lines)

      logpath = "Submits/logs/" + runname
      if not os.path.exists("Submits/logs/log" + runname):
        os.makedirs("Submits/logs/log" + runname)

      eossubfolder = EOSFolderName + "/"
      jobsubfolder = year + "_" + sampletype + "_" + trigger + "/"
      jobpath = "/eos/user/s/siluo/WPrimeAnalysis/" + eossubfolder + jobsubfolder
      if not os.path.exists(jobpath):
        os.makedirs(jobpath)
