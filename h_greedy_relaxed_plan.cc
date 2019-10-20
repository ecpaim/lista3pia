#include "h_greedy_relaxed_plan.h"

#include "../option_parser.h"
#include "../plugin.h"

using namespace std;

namespace planopt_heuristics {
GreedyRelaxedPlanHeuristic::GreedyRelaxedPlanHeuristic(const options::Options &options)
    : Heuristic(options),
      relaxed_task(task_proxy) {
}

int GreedyRelaxedPlanHeuristic::compute_heuristic(const GlobalState &state) {
    relaxed_task.initial_state = relaxed_task.translate_state(state);

    vector<bool> proposition_reached(relaxed_task.propositions.size(), false);
    vector<bool> operator_used(relaxed_task.operators.size(), false);
    for (PropositionID id : relaxed_task.initial_state) {
        proposition_reached[id] = true;
    }

    // TODO: add your code for exercise 1 (c) here.
    int path_cost = 0;
    bool fim;
    bool op_found = false;
    bool eff_test;
    
    while(true){
        fim = true;
        //checks if it is goal state (if all values in current state are true)
        for( PropositionID id : relaxed_task.goal ){
            if(proposition_reached[id] == false){
                fim = false;
                break;
            }
        }
    
        if( fim == false){
            //searches for an applicable operator
            for(RelaxedOperator op : relaxed_task.operators){
                
                //checks all preconditions
                op_found = true;
                for (auto pre : op.preconditions) {
                    
                    if(proposition_reached[pre]==false){
                        op_found = false;
                        break;
                    }
                }
                eff_test = false;
                for (auto eff : op.effects) {
                    if(proposition_reached[eff] ==false){
                        eff_test = true;
                        break;
                    }
                    
                }
                if(op_found&&eff_test){
                    for (auto eff : op.effects) {
                        //applies the operator
                        proposition_reached[eff] = true;
                    }
                    path_cost += op.cost;
                    break;                
                }
                
            }
            if(op_found == false){
                return DEAD_END;
            }
        }
        else{
            return path_cost;
        }
    }
    
    //return DEAD_END;
}

static Heuristic *_parse(OptionParser &parser) {
    Heuristic::add_options_to_parser(parser);
    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new GreedyRelaxedPlanHeuristic(opts);
}

static Plugin<Heuristic> _plugin("planopt_greedy_relaxed", _parse);

}
