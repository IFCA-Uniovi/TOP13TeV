import copy
import ROOT
from ROOT import TH1


class makeDatacards:
    def __init__(self,path,samples):
        self.path    = path
        self.samples = samples
        self.sysList =   ['Normal'  , 'BtagUp'    , 'BtagDown', 
                          'MisTagUp', 'MisTagDown', 'JESUp',
                          'JESDown' , 'JER'       , 'LESUp',
                          'LESDown' , 'PUUp'      , 'PUDown',
                          'TopPt']


    def parseSamples(self):
        self.procc = {}
        with open(self.samples) as f:
            for lin in f.readlines():
                if '#' in lin: continue
                if not ':' in lin: continue
                cosa = lin.split(':')
                proc  = cosa[0].strip(' ').replace('\n','')
                sampl = cosa[1].strip(' ').replace('\n','')
                if proc in self.procc:
                    self.procc[proc].append(sampl)
                else:
                    self.procc[proc] = [sampl]

    def getHistos(self):
        # histos is a dictionary. 
        # its keys are the processes, its elements are dictionaries of each systematic
        self.histos = {}
        for key, process in self.procc.iteritems():
            hists = {}
            for sys in self.sysList:
                for idx, sample in enumerate(process):
                    tfile = ROOT.TFile.Open(path+'/Tree_{sample}.root'.format(sample=sample))
                    if idx == 0:
                        hist = copy.deepcopy(tfile.Get('H_FitVariable_'+sys))
                    else:
                        hist.Add(copy.deepcopy(tfile.Get('H_FitVariable_'+sys)))
                    tfile.Close()
                hists[sys] = hist
            self.histos[key] = hists
        self.makeStatUnc()

    def makeStatUnc(self):
        for key, hists in self.histos.iteritems():
            nom = hists['Normal']
            for key2,hists2 in self.histos.iteritems():
                for bin in range(1,nom.GetNbinsX()+1):
                    varUp = copy.deepcopy(nom); varUp.SetName(nom.GetName()+'Stat'+key2+'Up')
                    varDn = copy.deepcopy(nom); varUp.SetName(nom.GetName()+'Stat'+key2+'Down')
                    if key == key2:
                        varUp.SetBinContent(bin, nom.GetBinContent(bin)+nom.GetBinError(bin))
                        varDn.SetBinContent(bin, nom.GetBinContent(bin)-nom.GetBinError(bin))
                        self.sysList.append('Stat_%d'%bin+key+'Up')
                        self.sysList.append('Stat_%d'%bin+key+'Down')
                    hists['Stat_%d'%bin+key2+'Up']   = varUp
                    hists['Stat_%d'%bin+key2+'Down'] = varDn


    def makeCards(self):
        # con un par de huevos
 
                
        lumi = 35800
        datacard = '''imax *
jmax *
kmax *
---------------
shapes * * shapes.root $PROCESS $PROCESS_$SYSTEMATIC
---------------
bin 1
observation {obs}
------------------------------\n'''.format(obs=0.)
        nProcesses = len(self.histos)
        line = 'bin         '
        for i in range(nProcesses):
            line = line + '1\t'
        line = line + '\n'
        datacard = datacard + line

        line1 = 'process     '
        line1 = line1 + 'TW\t'
        line2 = 'process     '
        line2 = line2 + '0\t'
        line3 = 'rate        '
        line3 = line3 + '%4.3f\t'%(self.histos['TW']['Normal'].Integral()*lumi)
        counter = 1
        
        for key, proc in self.histos.iteritems():
            if key == 'TW': continue
            line1 = line1 + key + '\t'
            line2 = line2 + '%d'%counter + '\t'
            line3 = line3 + '%4.3f\t'%(proc['Normal'].Integral()*lumi)
            counter = counter + 1
        line1 = line1 + '\n'
        line2 = line2 + '\n'
        line3 = line3 + '\n'
        datacard = datacard + line1 + line2 + line3
        datacard = datacard + '------------------------------\n'
        for sys in self.sysList:
            if sys == 'Normal': continue
            if 'Down' in sys  : continue
            sys = sys.replace('Up','')
            line = sys + '\t shape \t'
            for key, proc in self.histos.iteritems():
                if 'Stat' in sys and key in sys:
                    line = line + '1\t'
                elif 'Stat' in sys:
                    line = line + '0\t'
                else:
                    line = line + '1\t'
                
            line = line + '\n'
            datacard = datacard + line
#        datacard = datacard 
        print datacard
        fil = open('datacard.txt','w')
        fil.write(datacard)
        fil.close()

    def makeRootFile(self):
        lumi = 35800
        f = ROOT.TFile.Open('shapes.root','recreate')
        for key, proc in self.histos.iteritems():
#            print proc
            for sys in self.sysList:
                if sys == 'Normal':
                    proc[sys].SetName(key)
                else:
                    proc[sys].SetName(key+'_'+sys)
                proc[sys].Scale(lumi)
                proc[sys].Write()
        ###################################################
        kk = copy.deepcopy(self.histos['TTbar']['Normal'])
        kk.SetName('data_obs')
        kk.Write()
        ###################################################
        f.Close()

if __name__ == '__main__':
    path = '/mnt_pool/fanae105/user/sscruz/TW/TOP13TeV/temp'
    samples = 'samples.dat'
    a = makeDatacards(path,samples)
    a.parseSamples()
    a.getHistos()
    a.makeCards()
    a.makeRootFile()
    global lumi
    lumi = 36800
