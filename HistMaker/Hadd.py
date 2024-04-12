import os
import subprocess
import sys
import glob

def main(current_terminal, total_terminals):
    p = "/eos/user/m/mkizilov/WPrimeAnalysis/ValidationFitted/Hists/"
    pre = "Validation"
    j = "2017"

    #items = ["LeptonPt", "LeptonEta", "LeptonPhi", "Jet0Pt", "Jet0Eta", "Jet0Phi", "Jet1Pt", "Jet1Eta", "Jet1Phi", "Jet2Pt", "Jet2Eta", "Jet2Phi", "Jet3Pt", "Jet3Eta", "Jet3Phi", "Jet4Pt", "Jet4Eta", "Jet4Phi", "METPt", "METPhi", "dPhiMetLep", "mT", "HT", "ST", "WPrimeMassSimpleFL", "WPrimeMassSimpleLL", "WPrimeMass", "WPrimeMassFL", "WPrimeMassLL", "Likelihood", "LikelihoodCorrect", "LikelihoodEffCorrect", "LikelihoodInCorrect", "dR(Jet0,Jet1)", "dR(Jet0,Jet2)", "dR(Jet0,Jet3)", "dR(Jet0,Jet4)", "dR(Jet1,Jet2)", "dR(Jet1,Jet3)", "dR(Jet1,Jet4)", "dR(Jet2,Jet3)", "dR(Jet2,Jet4)", "dR(Jet3,Jet4)"]
    items = ["LeptonPt", "LeptonEta", "LeptonPhi", "Jet0Pt", "Jet0Eta", "Jet0Phi", "Jet1Pt", "Jet1Eta", "Jet1Phi", "Jet2Pt", "Jet2Eta", "Jet2Phi", "Jet3Pt", "Jet3Eta", "Jet3Phi", "Jet4Pt", "Jet4Eta", "Jet4Phi", "METPt", "METPhi", "dPhiMetLep", "mT", "HT", "ST", "WPrimeMassSimpleFL", "WPrimeMassSimpleLL", "WPrimeMass", "WPrimeMassFL", "WPrimeMassLL", "Likelihood", "dR(Jet0,Jet1)", "dR(Jet0,Jet2)", "dR(Jet0,Jet3)", "dR(Jet0,Jet4)", "dR(Jet1,Jet2)", "dR(Jet1,Jet3)", "dR(Jet1,Jet4)", "dR(Jet2,Jet3)", "dR(Jet2,Jet4)", "dR(Jet3,Jet4)"]
    #ks = ["ttbar", "SingleMuon","SingleElectron", "wjets_HT_70_100", "wjets_HT_100_200", "wjets_HT_200_400", "wjets_HT_400_600", "wjets_HT_600_800", "wjets_HT_800_1200", "wjets_HT_1200_2500", "wjets_HT_2500_inf", "single_top_schan", "single_top_tchan", "single_antitop_tchan", "single_top_tw", "single_antitop_tw", "WW", "ZZ", "WZTo1L1Nu2Q", "WZTo1L3Nu", "WZTo2Q2L", "WZTo3LNu", "FL300", "FL400", "FL500", "FL600", "FL700", "FL800", "FL900", "FL1000", "FL1100", "LL300", "LL400", "LL500", "LL600", "LL700", "LL800", "LL900", "LL1000", "LL1100"]
    ks = ["ttbar"]
    #items = ["ST"]
    # Split the items list into chunks for each terminal
    chunk_size = len(items) // total_terminals
    chunks = [items[i:i + chunk_size] for i in range(0, len(items), chunk_size)]

    # If there are any remaining items, add them to the last chunk
    if len(items) % total_terminals != 0:
        chunks[-1].extend(items[-(len(items) % total_terminals):])

    # Get the items for this terminal
    items_for_this_terminal = chunks[current_terminal - 1]

    for i in items_for_this_terminal:
        for k in ks:
            if k == "ttbar":
                # pre = "Validation_NRW"
                pre = "Validation_RW2On2"
            else: pre = "Validation"
            file_path = os.path.join(p, f"{j}_{pre}_{i}_{k}.root")
            if os.path.exists(file_path):
                os.remove(file_path)
            #subprocess.run(["hadd", "-f", file_path, os.path.join(p, f"{j}_{k}/{j}_{pre}_{i}_{k}_*")])
            file_paths = glob.glob(p + f"{j}_{k}/{j}_{pre}_{i}_{k}_*")

            subprocess.run(["hadd", "-f", file_path] + file_paths)
            print(f"hadd -f {os.path.join(p, f'{j}_{pre}_{i}_{k}.root')} {os.path.join(p, f'{j}_{k}/{j}_{pre}_{i}_{k}_*')}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 script.py <current_terminal> <total_terminals>")
        sys.exit(1)

    current_terminal = int(sys.argv[2])
    total_terminals = int(sys.argv[1])

    main(current_terminal, total_terminals)