#include <stdio.h>
#include "mod.h"




void stack_observer(void * p_ctx)
{
    printf("    stack_observer(%p)\n", p_ctx);
}

static nrf_sdh_req_observer_t const m_stack_observer_0 __attribute__ ((section(".sdh_stack_observers"))) __attribute__((used)) = {
        .handler = stack_observer,
        .p_context = (void *)NULL,
};

static nrf_sdh_req_observer_t const m_stack_observer_1 __attribute__ ((section(".sdh_stack_observers"))) __attribute__((used)) = {
        .handler = stack_observer,
        .p_context = (void *)1,
};

static nrf_sdh_req_observer_t const m_stack_observer_9 __attribute__ ((section(".sdh_stack_observers"))) __attribute__((used)) = {
        .handler = stack_observer,
        .p_context = (void *)9,
};


#if defined(TEST_WITH_ONE_OBSERVER_IS_OK)
void soc_observer(void * p_ctx)
{
    printf("    soc_observer(%p)\n", p_ctx);
}

static nrf_sdh_req_observer_t const m_soc_observer_0 __attribute__ ((section(".sdh_soc_observers"))) __attribute__((used)) = {
        .handler = soc_observer,
        .p_context = (void *)NULL,
};
#endif
