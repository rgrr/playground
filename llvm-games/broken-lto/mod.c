#include <stdio.h>

#include "mod.h"




void stack_observer(void * p_ctx)
{
    printf("    stack_observer(%p)\n", p_ctx);
}

NRF_SDH_STACK_OBSERVER( m_stack_observer_0, 0 ) = {
        .handler = stack_observer,
        .p_context = (void *)NULL,
};

NRF_SDH_STACK_OBSERVER( m_stack_observer_1, 0 ) = {
        .handler = stack_observer,
        .p_context = (void *)1,
};

NRF_SDH_STACK_OBSERVER( m_stack_observer_9, 0 ) = {
        .handler = stack_observer,
        .p_context = (void *)9,
};



void req_observer(void * p_ctx)
{
    printf("    req_observer(%p)\n", p_ctx);
}

NRF_SDH_REQUEST_OBSERVER( m_req_observer_0, 0 ) = {
        .handler = req_observer,
        .p_context = (void *)NULL,
};


void state_observer(void * p_ctx)
{
    printf("    state_observer(%p)\n", p_ctx);
}

NRF_SDH_STATE_OBSERVER( m_state_observer_0, 0 ) = {
        .handler = state_observer,
        .p_context = (void *)NULL,
};



void hello_mod(void)
{
    printf("Hello mod\n");
}
