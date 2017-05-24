#include "RecoTauTag/HLTProducers/interface/PFJetsTauOverlapRemoval.h"
#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/TauReco/interface/PFTau.h"

//
// class declaration
//
using namespace reco   ;
using namespace std    ;
using namespace edm    ;
using namespace trigger;

PFJetsTauOverlapRemoval::PFJetsTauOverlapRemoval(const edm::ParameterSet& iConfig):
  tauSrc    ( consumes<trigger::TriggerFilterObjectWithRefs>(iConfig.getParameter<InputTag>("TauSrc"      ) ) ),
  PFJetSrc( consumes<PFJetCollection>(iConfig.getParameter<InputTag>("PFJetSrc") ) )
{  
  produces<PFJetCollection>();
}
PFJetsTauOverlapRemoval::~PFJetsTauOverlapRemoval(){ }

void PFJetsTauOverlapRemoval::produce(edm::StreamID iSId, edm::Event& iEvent, const edm::EventSetup& iES) const
{
    
  unique_ptr<PFJetCollection> cleanedPFJets(new PFJetCollection);
    
  double deltaR    = 1.0;
  double matchingR = 0.5;
  
  // Getting HLT jets to be matched
  edm::Handle<trigger::TriggerFilterObjectWithRefs> tauJets;
  iEvent.getByToken( tauSrc, tauJets );
  
  edm::Handle<PFJetCollection> PFJets;
  iEvent.getByToken(PFJetSrc,PFJets);
                
  trigger::VRpftau taus; 
  tauJets->getObjects(trigger::TriggerTau,taus);
  
  if(edm::InputTag("PFJetSrc") == edm::InputTag("L1TPFJetMatching","TwoJets")){

    for(unsigned int iTau = 0; iTau < taus.size(); iTau++){  
      for(unsigned int iJet = 0; iJet < PFJets->size(); iJet++){
        // Find the relative L2TauJets, to see if it has been reconstructed
//        const PFTau &  myTau = (*tauJets)[iTau];
        const PFJet &  myPFJet = (*PFJets)[iJet];
        deltaR = ROOT::Math::VectorUtil::DeltaR((taus[iTau])->p4().Vect(), myPFJet.p4().Vect());
//        std::cout<<myPFJet.p4().pt()<<" ";
        if(deltaR > matchingR ) {
          cleanedPFJets->push_back(myPFJet);
        }
        break;
      }
    }
  }
  if(edm::InputTag("PFJetSrc") == edm::InputTag("L1TPFJetMatching","ThreeJets")){

    for(unsigned int iTau = 0; iTau < taus.size(); iTau++){  
      for(unsigned int iJet = 0; iJet < PFJets->size(); iJet++){
        // Find the relative L2TauJets, to see if it has been reconstructed
//        const PFTau &  myTau = (*tauJets)[iTau];
        const PFJet &  myPFJet = (*PFJets)[iJet];
        deltaR = ROOT::Math::VectorUtil::DeltaR((taus[iTau])->p4().Vect(), myPFJet.p4().Vect());
//        std::cout<<myPFJet.p4().pt()<<" ";
        if(deltaR > matchingR ) {
          cleanedPFJets->push_back(myPFJet);
        }
        break;
      }
    }
  }
 
  
  iEvent.put(std::move(cleanedPFJets));
}

void PFJetsTauOverlapRemoval::fillDescriptions(edm::ConfigurationDescriptions& descriptions) 
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("PFJetSrc", edm::InputTag("hltAK4PFJetsCorrectedIDPassed"                     ))->setComment("Input collection of PFJets"    );
  desc.add<edm::InputTag>("TauSrc"      , edm::InputTag("hltPFTau20TrackLooseIso"))->setComment("Input collection of PFTaus");
//  descriptions.setComment("This module produces collection of PFTaus matched to L1 Taus / Jets passing a HLT filter (Only p4 and vertex of returned PFTaus are set).");
  descriptions.add       ("PFJetsTauOverlapRemoval",desc);
}
