
int main()
{
    //char buffer[] = "MDM_SN : 352913090128354\nMDM_MDL : WP7608\nMFN : Sierra Wireless, Incorporated\nFMWV : SWI9X07Y_02.37.03.00 73df45 jenkins 2020/04/08 10:59:14\nCSQ :  28,99\nPower amp tmp : 43.00\nPower core tmp : 43.00";
    char buffer[]= "Periodic status : \nTue Oct 27 14:06:07 2020\nMDM_SN : 352913090128354\nMDM_MDL : WP7608\nMFN : Sierra_Wireless__Incorporated\nIMSI : 404450959353533\nCCID : 89914509009593535332\nSPN : airtel\nSYS_UPTIME : uptime=2020-10-27 10:01:25;diff_time=00:3;diff_day=0;\nVER_MDM_FMW : SWI9X07Y_02.37.03.00 73df45 jenkins 2020/04/08 10:59:14\nVER_RTR_FMW : SWI9X07Y_02.37.07.00__3.18.140\nVER_APP_RLS : 1.00\nVER_APP_INV : 1.0.0.1\nCSQ : 16,99\nPower amp tmp : 37.00\nPower core tmp : 37.00\nNET_STS : 1\nPNG_STS : 1\nIP_ecm0 : 192.168.2.2\nIP_eth0 : 10.1.1.105\nIP_eth1 : 192.168.3.2\nIP_eth2 : 10.1.1.107\nMEM_INFO : Total=158484; used=50.506042; cached=22.137251\nRTC_TM : 27092020140611";
    //char buffer[]="hgcccccccccccccc";
    int temp1 = 0, ret = 0;
    char delimiter = '\n';
    char *dest[35]={0};
    sVEGA_PRD_ obj;
    temp1 = split_string(buffer,dest,&delimiter);
    Parse_func_ parse_func[23];
    prs_mdm(parse_func);
    for(int i=0;i<23;i++)
    {
        parse_func[i].f_prs_mdm(dest,temp1,&obj);
    }
    return 0;
}
