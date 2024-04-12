import ROOT
import os

# Initialize TChain
chain = ROOT.TChain("t;1/RegionIdentifier")  # replace "tree" with the name of your TTree

# Path to your directory
path_to_files = "/eos/user/m/mkizilov/WPrimeAnalysis/ValidationFitted/2017_ttbar/"  # replace with your directory
i = 0
# Loop over all files in the directory
for filename in os.listdir(path_to_files):
    if filename.endswith(".root"):
        print(f"Adding file {filename} to the chain")
        chain.Add(os.path.join(path_to_files, filename))
        
#save the chain to a file
# Print number of events in the chain
print(f"Total number of events where RegionIdentifier is 1152: {chain.GetEntries('RegionIdentifier == 1152')}")

