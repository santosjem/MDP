#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/*  Procedure
 *    policy_evaluation
 *
 *  Purpose
 *    Iteratively estimate state utilities under a fixed policy
 *
 *  Parameters
 *   policy
 *   p_mdp
 *   epsilon
 *   gamma
 *   utilities
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    p_mdp is a pointer to a valid, complete mdp
 *    epsilon > 0
 *    0 < gamma < 1
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    utilities[s] has been updated according to the simplified Bellman update
 *    so that no update is larger than epsilon
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities)
{
  double * updated_utilities = malloc (sizeof(double) * p_mdp->numStates);
  double delta = 0;

  do {
    delta = 0;

    for(int state = 0; state < p_mdp->numStates; state++) {
      if(p_mdp->terminal[state]) {
        updated_utilities[state] = p_mdp->rewards[state];
      } else {
        double eu = calc_eu(p_mdp, state, utilities, policy[state]);
        updated_utilities[state] = p_mdp->rewards[state] + gamma * eu;
      }

      double fab = fabs(updated_utilities[state] - utilities[state]);
      
      if (fab > delta) {
        delta = fab;
      }
    }
      
    memcpy(utilities, updated_utilities, sizeof(double) * p_mdp->numStates);
    
  } while(delta <= epsilon);

  free(updated_utilities);
}
