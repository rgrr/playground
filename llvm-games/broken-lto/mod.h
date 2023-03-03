#pragma once


#include <stdbool.h>


#define STRINGIFY_(val) #val
#define STRINGIFY(val)  STRINGIFY_(val)

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
#define CONCAT_2_(p1, p2)     p1##p2


typedef void (*nrf_sdh_req_evt_handler_t)(void * p_context);


typedef struct
{
    nrf_sdh_req_evt_handler_t handler;      //!< Request handler.
    void *                    p_context;    //!< A parameter to the handler function.
} const nrf_sdh_req_observer_t;


#define NRF_SECTION_ITEM_REGISTER(section_name, section_var) \
    section_var __attribute__ ((section("." STRINGIFY(section_name)))) __attribute__((used))


#define NRF_SECTION_SET_ITEM_REGISTER(_name, _priority, _var)                                       \
    NRF_SECTION_ITEM_REGISTER(CONCAT_2(_name, _priority), _var)


#define NRF_SDH_REQUEST_OBSERVER(_observer, _prio)                                                  \
    NRF_SECTION_SET_ITEM_REGISTER(sdh_req_observers, _prio, nrf_sdh_req_observer_t const _observer)


#define NRF_SDH_STATE_OBSERVER(_observer, _prio)                                                           \
    NRF_SECTION_SET_ITEM_REGISTER(sdh_state_observers, _prio, static nrf_sdh_req_observer_t const _observer)


#define NRF_SDH_STACK_OBSERVER(_observer, _prio)                                                          \
    NRF_SECTION_SET_ITEM_REGISTER(sdh_stack_observers, _prio, static nrf_sdh_req_observer_t const _observer)


void hello_mod(void);
