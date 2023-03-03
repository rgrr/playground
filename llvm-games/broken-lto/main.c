#include <stdio.h>
#include "mod.h"



extern nrf_sdh_req_observer_t * __start_sdh_soc_observers;
extern void                   * __stop_sdh_soc_observers;
static nrf_section_set_t const sdh_soc_observers =
    {
        .section =
        {
            .p_start = &__start_sdh_soc_observers,
            .p_end   = &__stop_sdh_soc_observers,
        },
        .item_size  = sizeof(nrf_sdh_req_observer_t),
    };

extern nrf_sdh_req_observer_t * __start_sdh_stack_observers;
extern void                   * __stop_sdh_stack_observers;
static nrf_section_set_t const sdh_stack_observers =
    {
        .section =
        {
            .p_start = &__start_sdh_stack_observers,
            .p_end   = &__stop_sdh_stack_observers,
        },
        .item_size  = sizeof(nrf_sdh_req_observer_t),
    };



#if defined(TEST_INLINE_ITER_GET)
// this gives an "ld.lld-15: error: undefined symbol: nrf_section_iter_get" with -O0, if static then not...
inline void * nrf_section_iter_get(nrf_section_iter_t const * p_iter)
{
    return p_iter->p_item;
}   // nrf_section_iter_get
#else
#define nrf_section_iter_get(p_iter)   ((p_iter)->p_item)
#endif



void nrf_section_iter_init(nrf_section_iter_t * p_iter, nrf_section_set_t const * p_set)
{
    p_iter->p_set = p_set;

    p_iter->p_item = p_iter->p_set->section.p_start;
    if (p_iter->p_item == p_iter->p_set->section.p_end)
    {
        p_iter->p_item = NULL;
    }
}   // nrf_section_iter_init



void nrf_section_iter_next(nrf_section_iter_t * p_iter)
{
    if (p_iter->p_item == NULL)
    {
        return;
    }

    p_iter->p_item = (void *)((size_t)(p_iter->p_item) + p_iter->p_set->item_size);

    if (p_iter->p_item == p_iter->p_set->section.p_end)
    {
        p_iter->p_item = NULL;
    }
}   // nrf_section_iter_next



static void sdh_observer_notify(const char *name, nrf_section_set_t const *observer)
{
    nrf_section_iter_t iter;

    printf("%s ------------ %p %p\n", name, observer->section.p_start, observer->section.p_end);

    for (nrf_section_iter_init(&iter, observer);
         nrf_section_iter_get(&iter) != NULL;
         nrf_section_iter_next(&iter))
    {
        nrf_sdh_req_observer_t    * p_observer;
        nrf_sdh_req_evt_handler_t   handler;

        p_observer = (nrf_sdh_req_observer_t *) nrf_section_iter_get(&iter);
        handler    = p_observer->handler;

        handler(p_observer->p_context);
    }
}   // sdh_request_observer_notify



int main(int argc, char *argv[])
{
    sdh_observer_notify("sdh_stack_observers", &sdh_stack_observers);
    sdh_observer_notify("sdh_soc_observers", &sdh_soc_observers);
    return 0;
}
