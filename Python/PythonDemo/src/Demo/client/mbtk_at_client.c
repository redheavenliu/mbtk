abc

sdfsdf


11111111111111


mbtk_at_state mbtk_at_handle_MTEST1(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    
    return MBTKAT_OK;
}

mbtk_at_state mbtk_at_handle_MTEST2(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    
    return MBTKAT_OK;
}

mbtk_at_state mbtk_at_handle_MTEST3(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    
    return MBTKAT_OK;
}

mbtk_at_state mbtk_at_handle_MTEST4(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    
    return MBTKAT_OK;
}

mbtk_at_state mbtk_at_handle_MTEST5(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    
    return MBTKAT_OK;
}


/*
* ATI : 
* Support function:EXE
*/
mbtk_at_state mbtk_at_handle_I(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    mbtk_at_state result = MBTKAT_OK;
    switch(req->type)
    {
        case MBTKAT_TYPE_EXE:
        {

            break;
        }
        default:
        {
            LOGE("Unknow AT type.");
            result = MBTKAT_ERROR;
        }
    }
    return result;
}

/*
* AT+S1 : (0 1)(1-10)(s10)
* Support function:EXE/SET/READ/TEST
*/
mbtk_at_state mbtk_at_handle_S1(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)
{
    mbtk_at_state result = MBTKAT_OK;
    switch(req->type)
    {
        case MBTKAT_TYPE_EXE:
        {

            break;
        }
        case MBTKAT_TYPE_SET:
        {

            break;
        }
        case MBTKAT_TYPE_READ:
        {

            break;
        }
        case MBTKAT_TYPE_TEST:
        {

            break;
        }
        default:
        {
            LOGE("Unknow AT type.");
            result = MBTKAT_ERROR;
        }
    }
    return result;
}
