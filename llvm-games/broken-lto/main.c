#include <stdio.h>
#include "mod.h"


__attribute__ ((section (".hash"))) const char hash[] = "helloimahash";




void call_observer(const char *name, const nrf_sdh_req_observer_t *start, const nrf_sdh_req_observer_t *stop)
{
    const nrf_sdh_req_observer_t *p;

    printf("%s ------------ %p %p\n", name, start, stop);
    for (p = start;  p < stop;  ++p)
    {
        p->handler(p->p_context);
    }
}   // call_observer



typedef struct
{
    void * p_start;     //!< Pointer to the start of section.
    void * p_end;       //!< Pointer to the end of section.
} nrf_section_t;

typedef struct
{
    nrf_section_t           section;    //!< Description of the set of sections.
    size_t                  item_size;  //!< Size of the single item in the section.
} nrf_section_set_t;

typedef struct
{
    nrf_section_set_t const * p_set;        //!< Pointer to the appropriate section set.
    void                    * p_item;       //!< Pointer to the selected item in the section.
} nrf_section_iter_t;


#define NRF_SECTION_START_ADDR(section_name)       &CONCAT_2(__start_, section_name)
#define NRF_SECTION_END_ADDR(section_name)         &CONCAT_2(__stop_, section_name)

#define NRF_SECTION_DEF(section_name, data_type)                \
    extern data_type * CONCAT_2(__start_, section_name);        \
    extern void      * CONCAT_2(__stop_,  section_name)

#define NRF_SECTION_SET_DEF(_name, _type)                                                           \
                                                                                                    \
    NRF_SECTION_DEF(_name, _type);                                                                  \
    static nrf_section_set_t const _name =                                                          \
    {                                                                                               \
        .section =                                                                                  \
        {                                                                                           \
            .p_start = NRF_SECTION_START_ADDR(_name),                                               \
            .p_end   = NRF_SECTION_END_ADDR(_name),                                                 \
        },                                                                                          \
        .item_size  = sizeof(_type),                                                                \
    }


NRF_SECTION_SET_DEF(sdh_soc_observers, nrf_sdh_req_observer_t);
NRF_SECTION_SET_DEF(sdh_ble_observers, nrf_sdh_req_observer_t);
NRF_SECTION_SET_DEF(sdh_state_observers, nrf_sdh_req_observer_t);
NRF_SECTION_SET_DEF(sdh_stack_observers, nrf_sdh_req_observer_t);
NRF_SECTION_SET_DEF(sdh_req_observers, nrf_sdh_req_observer_t);



#if 1
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
    printf("Hello main '%s'\n", hash);
    hello_mod();

    sdh_observer_notify("sdh_soc_observers", &sdh_soc_observers);
//    sdh_observer_notify("sdh_req_observers", &sdh_ble_observers);
    sdh_observer_notify("sdh_req_observers", &sdh_state_observers);
    sdh_observer_notify("sdh_req_observers", &sdh_stack_observers);
//    sdh_observer_notify("sdh_req_observers", &sdh_req_observers);

    return 0;
}
