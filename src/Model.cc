#include "Model.h" 

namespace model{
   template<>
   void run<WRIGHTFISHER,HAPLOID>(Population* &_src,Population* &_dst,Pool* &_pool){
      uniform_int_distribution<> uniform(0U,_src->size()-1U);
      uniform_real_distribution<double> mutation_rate(0.0,1.0);

      VirtualSequence* reference=nullptr;
      VirtualSequence* validator=nullptr;

      Individual* individual=nullptr;

      for(uint32_t id=0U;id<_dst->size();id++){
         individual=_src->at(uniform(rng));

         for(uint32_t cid=0;cid<individual->n_chromosomes();cid++){
            for(uint32_t gid=0;gid<individual->chromosome(cid)[0]->n_genes();gid++){
               if(mutation_rate(rng)<=individual->chromosome(cid)[0]->gene(gid)->mutation_rate()){
               		// Cosntructor de copia, al parecer
                  reference=new VirtualSequence(*(individual->chromosome(cid)[0]->gene(gid)->reference()));
                  reference->mutate();
						
						validator=_pool->push(cid,gid,reference);
                  if(validator!=nullptr){
                     delete reference;
                     reference=validator;
                  }
                  _dst->at(id)->chromosome(cid)[0]->gene(gid)->reference(reference);
               }
               else
                  _dst->at(id)->chromosome(cid)[0]->gene(gid)->reference(individual->chromosome(cid)[0]->gene(gid)->reference());
            }
         }
      }
   }
}
