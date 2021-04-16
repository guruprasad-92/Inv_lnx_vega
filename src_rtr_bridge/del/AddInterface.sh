#!/bin/sh
. /reap/lib/functions.sh

#
# PKG_RELEASE: 1.06
#

#RAT
# Read Modem Configuration file
# 
ReAPMqttHost="localhost"

ReAPMqttMsg="SIM_ABSENT"
ReAPMqttPort="1883"
ReAPMqttQos="1"
ReAPMqttNWKeventPubTopic="IOCard1/P500/C/SIMStatus"
SimConfigFile="/reap/etc/simconfig"
Retry="5"

ReadModemConfigFile()
{
    config_load "$ModemConfigFile"
    config_get ModemEnable "$InterfaceName" modemenable
    config_get DataEnable "$InterfaceName" dataenable
    config_get APN "$InterfaceName" apn
    config_get SIMSelection "$InterfaceName" simselection
    config_get ProfileIndex "$InterfaceName" profileindex
    config_get  "$InterfaceName" service
    config_get RAT1 "$InterfaceName" service
    config_get service1 "$InterfaceName" service1
    config_get RAT2 "$InterfaceName" service2
    config_get RAT3 "$InterfaceName" service3
    config_get CELLMODEM "$InterfaceName" cellular
    config_get auth "$InterfaceName" auth
    config_get gps "$InterfaceName" gps
    config_get cellular "$InterfaceName" cellular

    
    config_get Sim2ModemEnable "$InterfaceName" Sim2modemenable
    config_get Sim2DataEnable "$InterfaceName" Sim2dataenable
    config_get Sim2APN "$InterfaceName" Sim2apn
    config_get Sim2SIMSelection "$InterfaceName" Sim2simselection
    config_get Sim2ProfileIndex "$InterfaceName" Sim2profileindex
    config_get  "$InterfaceName" Sim2service
    config_get Sim2RAT1 "$InterfaceName" Sim2service
    config_get Sim2service1 "$InterfaceName" Sim2service1
    config_get Sim2RAT2 "$InterfaceName" Sim2service2
    config_get Sim2RAT3 "$InterfaceName" Sim2service3
    config_get Sim2CELLMODEM "$InterfaceName" Sim2cellular
    config_get Sim2auth "$InterfaceName" Sim2auth
    config_get Sim2gps "$InterfaceName" Sim2gps
    config_get pdp "$InterfaceName" pdp
    config_get Sim2pdp "$InterfaceName" Sim2pdp
     if [ "$cellular" = "WP7607-1" ]
     then
    {
	# echo "Sim1apn=$apn" 
	 echo "Sim1apn=\"$APN\""
	 echo "sim1service=\"$service1"\"
	 echo "Sim2apn=\"$Sim2APN"\" 
	 echo "sim2service=\"$Sim2service1"\"
	echo "sim1pdp=\"$pdp"\"
    echo "sim2pdp=\"$Sim2pdp"\"		
	} > "${SimConfigFile}.cfg"
	fi
}

#
# 
# 
AddNetworkInterface()
{
	RetryCount=1
	value=0

	
    if [ "x$DataEnable" != "x" ] && [ "$DataEnable" -eq "1" ]
    then
        #Enable GPS service
        if [ "$ModemEnable" -eq  "1" ] && [ "$DataEnable" -eq "1" ]
        then
            if [ "$gps" -eq "1" ]
		    then
		        echo 'AT!GPSAUTOSTART=1,1,180,50,1' | /reap/bin/atinout - /dev/ttyAT - 2>&1    
            elif [ "$gps" -eq "0" ]
            then               
                echo 'AT!GPSAUTOSTART=2,1,180,50,1' | /reap/bin/atinout - /dev/ttyAT -  2>&1
            fi                
        fi
    
        #Turn ON Modem Power
        ${CMapp} radio "on" >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error SIM1: Failed to power ON the modem" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
            mosquitto_pub -d -t "Isensev2/SimSwitch/CommandResponse" -m "<,1,SELSIM1,$CurrentDate,-1,1,$CurrentDate,>"  
            exit 1
        fi
         ${CMapp} sim select EMBEDDED >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error SIM1: Failed to Select EMBEDDED for SIM 2" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
		
		#echo 0 > /sys/class/gpio/gpio22/value
        #if [ $? -ne 0 ]
        #then
         #   echo "Error SIM1: Failed to toggle GPIO value to 1 for SIM 0" >> ${AddIfaceLogFile} 2>&1
          #  echo "status=Disabled" > "$AddIfaceStatusFile" 
           # exit 1
        #fi    
             
         select sim
        ${CMapp} sim select $SIMSelection >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error: Failed to Select SIM" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
        
        while [ $RetryCount -le $Retry ]
        do
	        #sim presence
	        SIMStatus=$(${CMapp} sim status)
	        echo "$SIMStatus" >> ${AddIfaceLogFile} 2>&1
	        pSIMStatus=$(echo $SIMStatus | cut -d "(" -f2 | cut -d ")" -f1)
	        if [ "$pSIMStatus" = "LE_SIM_READY" ]
	        then
	            break
	            
	        else
	            if [ $RetryCount = $Retry ]
	            then
					value=1
				fi
	        fi
	        RetryCount=$((RetryCount + 1))
		done
		
		if [ $value = "1" ]
		then
			echo "Error SIM1: Failed on SIM status" >> ${AddIfaceLogFile} 2>&1
	        echo "status=Disabled" > "$AddIfaceStatusFile" 
	        CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
            mosquitto_pub -d -t "Isensev2/SimSwitch/CommandResponse" -m "<,1,SELSIM1,$CurrentDate,-1,1,$CurrentDate,>"  
	        
	        #output=$(mosquitto_pub -h "$ReAPMqttHost" -p "$ReAPMqttPort" -t "$ReAPMqttNWKeventPubTopic" -m "$ReAPMqttMsg" -q $ReAPMqttQos)
			#retval=$?
			#if [ $retval = "0" ]
			#then
				#echo "mosquitto publish succcess"
			#fi
			echo 'SIMStatus="0"' > /tmp/SIMStatus.cfg 2>&1
	        exit 1
		else
	        echo 'SIMStatus="1"' > /tmp/SIMStatus.cfg 2>&1
		fi
        #Set the authentication protocol
		if [ "$auth" != "none" ]
        then
            echo "Authentication protocol is $auth"
        fi 
   
        #Set the profile index
        ${CMapp} data profile "$ProfileIndex"
            
        #Set the APN 
       ${CMapp} data apn "$APN"
            
        #Set Mode
		#if [ "$CELLMODEM" = "WP8548" ]
		#then
		#	if [ "$RAT" == "GSM" ]
		#	then
				#${CMapp} radio rat $RAT >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $RAT >> ${AddIfaceLogFile} 2>&1
		#	fi
		#elif [ "$CELLMODEM" = "WP7608" ]
		#then
		#	if [ "$RAT1" == "GSM" ]
		#	then
			      #	${CMapp} radio rat $RAT1 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
			      #	${CMapp} radio rat $RAT1 >> ${AddIfaceLogFile} 2>&1
		#		echo
		#	fi
		#elif [ "$CELLMODEM" = "WP7608-1" ]
		#then
		#	if [ "$RAT2" == "GSM" ]
		#	then
		#		${CMapp} radio rat $RAT2 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $RAT2 >> ${AddIfaceLogFile} 2>&1
		#	fi
		#elif [ "$CELLMODEM" = "WP7702" ]
		#then
		#	if [ "$RAT3" == "GSM" ]
		#	then
		#		${CMapp} radio rat $RAT3 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $RAT3 >> ${AddIfaceLogFile} 2>&1
		#	fi
			
	   #	fi
	   	   
	   
	   ${CMapp} radio rat $service1 >> ${AddIfaceLogFile} 2>&1
		
		if [ $? -ne 0 ]
        then
            echo "Error SIM1: Failed to set radio access technology" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
            mosquitto_pub -d -t "Isensev2/SimSwitch/CommandResponse" -m "<,1,SELSIM1,$CurrentDate,-1,1,$CurrentDate,>"  
            exit 1
        fi
        #Try to connect to data
        #/etc/init.d/PConnectionManager start >> ${AddIfaceLogFile} 2>&1
       # $ConnectionManagerApp > /dev/null 2>&1 &
       #$ConnectionManagerApp > /dev/null 2>&1 &
       
        if [ $? -ne 0 ]
        then
            echo "Error SIM1: Failed to connect" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
            mosquitto_pub -d -t "Isensev2/SimSwitch/CommandResponse" -m "<,1,SELSIM1,$CurrentDate,-1,1,$CurrentDate,>"  
            exit 1
        fi	
            
        #
        echo "status=Enabled" > "$AddIfaceStatusFile"
     else
        echo "status=Disabled" > "$AddIfaceStatusFile" 
         CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
            mosquitto_pub -d -t "Isensev2/SimSwitch/CommandResponse" -m "<,1,SELSIM1,$CurrentDate,-1,1,$CurrentDate,>"  
    fi
}

######################################################################################

#Add Interface for Second SIM


#########################################################################################

AddNetworkInterface2()
{
	RetryCount=1
	value=0

	
    if [ "x$Sim2DataEnable" != "x" ] && [ "$Sim2DataEnable" -eq "1" ]
    then
        #Enable GPS service
        if [ "$Sim2ModemEnable" -eq  "1" ] && [ "$Sim2DataEnable" -eq "1" ]
        then
            if [ "$Sim2gps" -eq "1" ]
		    then
		        echo 'AT!GPSAUTOSTART=1,1,180,50,1' | /reap/bin/atinout - /dev/ttyAT - 2>&1    
            elif [ "$Sim2gps" -eq "0" ]
            then               
                echo 'AT!GPSAUTOSTART=2,1,180,50,1' | /reap/bin/atinout - /dev/ttyAT -  2>&1
            fi                
        fi
    
        #Turn ON Modem Power
        ${CMapp} radio "on" >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error SIM2: Failed to power ON the modem for SIM2" >> ${AddIfaceLogFile} 2>&1
            echo "status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
        
         select sim
        ${CMapp} sim select EMBEDDED >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error SIM2: Failed to Select EMBEDDED for SIM 2" >> ${AddIfaceLogFile} 2>&1
            echo "Sim2status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
        
  
        ${CMapp} sim select $Sim2SIMSelection >> ${AddIfaceLogFile} 2>&1
        if [ $? -ne 0 ]
        then
            echo "Error SIM2: Failed to Select SIM 2" >> ${AddIfaceLogFile} 2>&1
            echo "Sim2status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
        
        while [ $RetryCount -le $Retry ]
        do
	        #sim presence
	        Sim2SIMStatus=$(${CMapp} sim status)
	        echo "$Sim2SIMStatus" >> ${AddIfaceLogFile} 2>&1
	        Sim2pSIMStatus=$(echo $Sim2SIMStatus | cut -d "(" -f2 | cut -d ")" -f1)
	        if [ "$Sim2pSIMStatus" = "LE_SIM_READY" ]
	        then
	            break
	            
	        else
	            if [ $RetryCount = $Retry ]
	            then
					value=1
				fi
	        fi
	        RetryCount=$((RetryCount + 1))
		done
		
		if [ $value = "1" ]
		then
			echo "Error SIM2: Failed on SIM 2 status" >> ${AddIfaceLogFile} 2>&1
	        echo "Sim2status=Disabled" > "$AddIfaceStatusFile" 
	        #output=$(mosquitto_pub -h "$ReAPMqttHost" -p "$ReAPMqttPort" -t "$ReAPMqttNWKeventPubTopic" -m "$ReAPMqttMsg" -q $ReAPMqttQos)
			#retval=$?
			#if [ $retval = "0" ]
			#then
				#echo "mosquitto publish succcess"
			#fi
			echo 'Sim2SIMStatus="0"' > /tmp/SIMStatus.cfg 2>&1
	        exit 1
		else
	        echo 'Sim2SIMStatus="1"' > /tmp/SIMStatus.cfg 2>&1
		fi
        #Set the authentication protocol
		if [ "$Sim2auth" != "none" ]
        then
            echo "Authentication protocol is $Sim2auth"
        fi 
   
        #Set the profile index
        ${CMapp} data profile "$Sim2ProfileIndex"
            
        #Set the APN 
       # ${CMapp} data apn "$Sim2APN"
            
        #Set Mode
		#if [ "$Sim2CELLMODEM" = "WP8548" ]
		#then
		#	if [ "$Sim2RAT" == "GSM" ]
		#	then
		#		${CMapp} radio rat $Sim2RAT >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $Sim2RAT >> ${AddIfaceLogFile} 2>&1
		#	fi
		#elif [ "$Sim2CELLMODEM" = "WP7608" ]
		#then
		#	if [ "$Sim2RAT1" == "GSM" ]
		#	then
		#		${CMapp} radio rat $Sim2RAT1 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $Sim2RAT1 >> ${AddIfaceLogFile} 2>&1
		#	fi
		#elif [ "$Sim2CELLMODEM" = "WP7608-1" ]
		#then
		#	if [ "$Sim2RAT2" == "GSM" ]
		#	then
		#		${CMapp} radio rat $Sim2RAT2 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $Sim2RAT2 >> ${AddIfaceLogFile} 2>&1
		#	fi
		#elif [ "$Sim2CELLMODEM" = "WP7702" ]
		#then
		#	if [ "$Sim2RAT3" == "GSM" ]
		#	then
		#		${CMapp} radio rat $Sim2RAT3 >> ${AddIfaceLogFile} 2>&1
		#		sleep 5
		#	else
		#		${CMapp} radio rat $Sim2RAT3 >> ${AddIfaceLogFile} 2>&1
		#	fi
			
		#fi
		${CMapp} radio rat $Sim2service1 >> ${AddIfaceLogFile} 2>&1

		
		if [ $? -ne 0 ]
        then
            echo "Error SIM2: Failed to set radio access technology For SIM2" >> ${AddIfaceLogFile} 2>&1
            echo "Sim2status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
        fi
        #Try to connect to data
        #/etc/init.d/PConnectionManager start >> ${AddIfaceLogFile} 2>&1
        $ConnectionManagerApp > /dev/null 2>&1 &
	
        if [ $? -ne 0 ]
        then
            echo "Error SIM2: Failed to connect SIM2" >> ${AddIfaceLogFile} 2>&1
            echo "Sim2status=Disabled" > "$AddIfaceStatusFile" 
            exit 1
	else
	
        sleep 5
	CurrentDate=$(date +"%Y-%m-%d %H:%M:%S")
        mosquitto_pub -d -t "Isensev2/SimSwitch/CommandRequest" -m "<,1,SELSIM1,$CurrentDate,>"
        fi
        
        #
        echo "Sim2status=Enabled" > "$AddIfaceStatusFile"
        echo "1" > "$ActiveSIMStatusFile"
     else
        echo "Sim2status=Disabled" > "$AddIfaceStatusFile"
        echo "error" > "$ActiveSIMStatusFile"
    fi
}


#
# Input arguments and Default Parameters
#

InterfaceName="rmnet_data0"
ScriptStatustDir="/tmp/InterfaceManager/status"
CMapp=/mnt/legato/system/bin/cm
#APP=/mnt/legato/system/bin/app
ModemConfigFile=/reap/etc/config/modem
ScriptLogDir="/tmp/InterfaceManager/log"
ActiveSIMStatusDir="/reap/etc/config"
AddIfaceLogFile="$ScriptLogDir/AddIfaceStatus.log"
AddIfaceStatusFile="$ScriptStatustDir/$InterfaceName"".AddIfaceStatus"
DeleteStatusFile="$ScriptStatustDir/$InterfaceName"".DeleteIfaceStatus"
#This file will update details of active SIM : 1 - SIM1 2-SIM2
ActiveSIMStatusFile="$ActiveSIMStatusDir/""ActiveSIMStatus"
ConnectionManagerApp="/legato/systems/current/apps/RelCellularManagerApp/read-only/bin/RelCellularManagerAppComponentExe"
App="RelCellularManagerAppComponentExe"

[ -d "$ScriptLogDir" ]  || mkdir -p "$ScriptLogDir"
[ -d "$ScriptStatustDir" ]  || mkdir -p "$ScriptStatustDir"
rm -f ${AddIfaceLogFile}

#
# 
#

# disconnect data

PSOutput=$(ps x)
if echo "$PSOutput" | grep -q "$ConnectionManagerApp"
then
#/etc/init.d/PConnectionManager stop >> ${AddIfaceLogFile} 2>&1
killall -9 "$App" > /dev/null 2>&1
fi

ReadModemConfigFile
rm -f "$DeleteStatusFile"
if [ "$ModemEnable" -ne 1 ]
then
    #Turn OFF Modem Power
    ${CMapp} radio "off"  >> ${AddIfaceLogFile} 2>&1
    if [ $? -ne 0 ]
    then
        echo "Error: Failed to power OFF the modem" >> ${AddIfaceLogFile} 2>&1
    fi
   # rm -f "$AddIfaceStatusFile"
    exit 0
fi
AddNetworkInterface

#Add Second SIM

if [ "$Sim2ModemEnable" -ne 1 ]
then
    #Turn OFF Modem Power
    ${CMapp} radio "off"  >> ${AddIfaceLogFile} 2>&1
    if [ $? -ne 0 ]
    then
        echo "Error: Failed to power OFF the modem" >> ${AddIfaceLogFile} 2>&1
    fi
   # rm -f "$AddIfaceStatusFile"
    exit 0
fi
AddNetworkInterface2

exit 0
