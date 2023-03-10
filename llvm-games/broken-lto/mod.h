#pragma once


#include <stdbool.h>


typedef void (*nrf_sdh_req_evt_handler_t)(void * p_context);


typedef struct
{
    nrf_sdh_req_evt_handler_t handler;      //!< Request handler.
    void *                    p_context;    //!< A parameter to the handler function.
} const nrf_sdh_req_observer_t;


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
