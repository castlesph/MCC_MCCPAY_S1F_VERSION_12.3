package com.Source.S1_MCCPAY.MCCPAY.Main;

import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

public class Encryption {
    public static final String TAG = Encryption.class.getSimpleName();

    public String TLE_Encrypt(String Data, String Key){
        String encrypted = "";
		String key_first_part = Key.substring(0,16);
        String key_second_part = Key.substring(16,32);
        byte[] baEncrypted= new byte[1028];
        //String sKey1 = "6FB79CA8850EB389";

        byte[] encryptedText = Data.getBytes();
        try{
            //byte[] baKey1 = "411ABEE64B73A6B3".getBytes();
            byte[] baKey1 = key_first_part.getBytes();
            byte[] baKey1BCD = bin_to_bcd(baKey1);

            //byte[] baKey2 = "6FB79CA8850EB389".getBytes();
            byte[] baKey2 = key_second_part.getBytes();
            byte[] baKey2BCD = bin_to_bcd(baKey2);

            byte[] baData = Data.getBytes();
            byte[] baDataBCD = bin_to_bcd(baData);

            DESKeySpec keySpec1 = new DESKeySpec(baKey1BCD);
            SecretKeyFactory keyFactory1 = SecretKeyFactory.getInstance("DES");
            SecretKey key1 = keyFactory1.generateSecret(keySpec1);

            DESKeySpec keySpec2 = new DESKeySpec(baKey2BCD);
            SecretKeyFactory keyFactory2 = SecretKeyFactory.getInstance("DES");
            SecretKey key2 = keyFactory2.generateSecret(keySpec2);



            //Cipher cipher = Cipher.getInstance("DES"); // cipher is not thread safe
            //Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding", "BC");
            Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");

            int inLen = baDataBCD.length, inIndex=0;
            int inIndex2=8, inDespost=0;
            //

            baEncrypted = new byte[inLen];
            //System.out.println(TAG + "baDataBCD.length:" + inLen);
            for(inIndex=0; inIndex<((inLen)/8); inIndex++) {
                //System.arraycopy(baDataBCD, (inIndex*8), cleartext, 0, 8);
                //cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), 8);
                byte[] cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), inIndex2);
                inIndex2+=8;

                //System.out.println(TAG + String.format(" cleartext: %02X%02X%02X%02X%02X%02X%02X%02X", cleartext[0], cleartext[1], cleartext[2], cleartext[3], cleartext[4], cleartext[5], cleartext[6], cleartext[7]));

                cipher.init(Cipher.ENCRYPT_MODE, key1);
                byte[] Encrypt1 = cipher.doFinal(cleartext);

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.DECRYPT_MODE, key2);
                byte[] Decrypt = cipher.doFinal(Encrypt1);

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.ENCRYPT_MODE, key1);
                byte[] Encrypt2 = cipher.doFinal(Decrypt);

                //System.out.println(TAG + String.format(" Encrypt2: %02X%02X%02X%02X%02X%02X%02X%02X", Encrypt2[0], Encrypt2[1], Encrypt2[2], Encrypt2[3], Encrypt2[4], Encrypt2[5], Encrypt2[6], Encrypt2[7]));
                //baEncrypted=Encrypt2;
                System.arraycopy(Encrypt2,0, baEncrypted, inIndex*8, 8);
                //inDespost+=8;
            }
            //encrypted= new String(baEncrypted);
        }catch (Exception e){

        }

        /*
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[0], baEncrypted[1], baEncrypted[2], baEncrypted[3], baEncrypted[4], baEncrypted[5], baEncrypted[6], baEncrypted[7]));
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[8], baEncrypted[9], baEncrypted[10], baEncrypted[11], baEncrypted[12], baEncrypted[13], baEncrypted[14], baEncrypted[15]));
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[24], baEncrypted[25], baEncrypted[26], baEncrypted[27], baEncrypted[28], baEncrypted[29], baEncrypted[30], baEncrypted[31]));
*/
        //System.out.println(TAG + " Encrypt2: %02X%02X%02X%02X%02X%02X%02X%02X", encrypted[0], encrypted[1], encrypted[2], Encrypt2[3], Encrypt2[4], Encrypt2[5], Encrypt2[6], Encrypt2[7]);
        //System.out.println("user_str baEncrypted.length" + baEncrypted.length);
        encrypted=bcd_to_bin(baEncrypted);
        return encrypted;
    }

    public String TLE_Decrypt(String Data, String Key){
        String encrypted = "";
		String key_first_part = Key.substring(0,16);
        String key_second_part = Key.substring(16,32);
        byte[] baEncrypted= new byte[1028];
        //String sKey1 = "6FB79CA8850EB389";

        byte[] encryptedText = Data.getBytes();
        try{
            //byte[] baKey1 = "411ABEE64B73A6B3".getBytes();
            byte[] baKey1 = key_first_part.getBytes();
            byte[] baKey1BCD = bin_to_bcd(baKey1);

            //byte[] baKey2 = "6FB79CA8850EB389".getBytes();
            byte[] baKey2 = key_second_part.getBytes();
            byte[] baKey2BCD = bin_to_bcd(baKey2);

            byte[] baData = Data.getBytes();
            //int inLength= baData.length-46;
            int inLength= baData.length;
            byte[] baData2 = new byte[inLength];
            //System.arraycopy(baData,46, baData2, 0, inLength);
            System.arraycopy(baData,0, baData2, 0, inLength);
            byte[] baDataBCD = bin_to_bcd(baData2);

            //System.out.println(TAG + String.format(" baDataBCD: %02X%02X%02X%02X%02X%02X%02X%02X", baDataBCD[0], baDataBCD[1], baDataBCD[2], baDataBCD[3], baDataBCD[4], baDataBCD[5], baDataBCD[6], baDataBCD[7]));

            DESKeySpec keySpec1 = new DESKeySpec(baKey1BCD);
            SecretKeyFactory keyFactory1 = SecretKeyFactory.getInstance("DES");
            SecretKey key1 = keyFactory1.generateSecret(keySpec1);

            DESKeySpec keySpec2 = new DESKeySpec(baKey2BCD);
            SecretKeyFactory keyFactory2 = SecretKeyFactory.getInstance("DES");
            SecretKey key2 = keyFactory2.generateSecret(keySpec2);

            //Cipher cipher = Cipher.getInstance("DES"); // cipher is not thread safe
            //Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding", "BC");
            Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");

            int inLen = baDataBCD.length, inIndex=0;
            int inIndex2=8, inDespost=0;
            //

            baEncrypted = new byte[inLen];
            //System.out.println(TAG + "baDataBCD.length:" + inLen);
            for(inIndex=0; inIndex<((inLen)/8); inIndex++) {
                //System.arraycopy(baDataBCD, (inIndex*8), cleartext, 0, 8);
                //cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), 8);
                byte[] cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), inIndex2);
                inIndex2+=8;

                //System.out.println(TAG + String.format(" cleartext: %02X%02X%02X%02X%02X%02X%02X%02X", cleartext[0], cleartext[1], cleartext[2], cleartext[3], cleartext[4], cleartext[5], cleartext[6], cleartext[7]));

                cipher.init(Cipher.DECRYPT_MODE, key1);
                byte[] Decrypt1 = cipher.doFinal(cleartext);

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.ENCRYPT_MODE, key2);
                byte[] Encrypt = cipher.doFinal(Decrypt1);

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.DECRYPT_MODE, key1);
                byte[] Decrypt2 = cipher.doFinal(Encrypt);

                //System.out.println(TAG + String.format(" Decrypt2: %02X%02X%02X%02X%02X%02X%02X%02X", Decrypt2[0], Decrypt2[1], Decrypt2[2], Decrypt2[3], Decrypt2[4], Decrypt2[5], Decrypt2[6], Decrypt2[7]));
                //baEncrypted=Encrypt2;
                System.arraycopy(Decrypt2,0, baEncrypted, inIndex*8, 8);
                //inDespost+=8;
            }
        }catch (Exception e){

        }

        /*
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[0], baEncrypted[1], baEncrypted[2], baEncrypted[3], baEncrypted[4], baEncrypted[5], baEncrypted[6], baEncrypted[7]));
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[8], baEncrypted[9], baEncrypted[10], baEncrypted[11], baEncrypted[12], baEncrypted[13], baEncrypted[14], baEncrypted[15]));
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[24], baEncrypted[25], baEncrypted[26], baEncrypted[27], baEncrypted[28], baEncrypted[29], baEncrypted[30], baEncrypted[31]));
*/
        //System.out.println(TAG + " Encrypt2: %02X%02X%02X%02X%02X%02X%02X%02X", encrypted[0], encrypted[1], encrypted[2], Encrypt2[3], Encrypt2[4], Encrypt2[5], Encrypt2[6], Encrypt2[7]);
        encrypted=bcd_to_bin(baEncrypted);
        return encrypted;
    }

    public String TLE_MAC(String Data, String Key){
        String encrypted = "";
        String key_first_part = Key.substring(16,32);
        String key_second_part = Key.substring(0,16);
        byte[] baEncrypted = new byte[2048];
        byte[] baEncrypMAC= new byte[2048];
        byte[] EncryptMAC = new byte[8];
        //String sKey1 = "6FB79CA8850EB389";

        //System.out.println(TAG + "key_first_part: "+key_first_part);
        //System.out.println(TAG + "key_second_part: "+key_second_part);

        byte[] encryptedText = Data.getBytes();
        try{
            //byte[] baKey1 = "6FB79CA8850EB389".getBytes();
            byte[] baKey1 = key_first_part.getBytes();
            byte[] baKey1BCD = bin_to_bcd(baKey1);

            //byte[] baKey2 = "411ABEE64B73A6B3".getBytes();
            byte[] baKey2 = key_second_part.getBytes();
            byte[] baKey2BCD = bin_to_bcd(baKey2);

            byte[] baData = Data.getBytes();
            byte[] baDataBCD = bin_to_bcd(baData);

            DESKeySpec keySpec1 = new DESKeySpec(baKey1BCD);
            SecretKeyFactory keyFactory1 = SecretKeyFactory.getInstance("DES");
            SecretKey key1 = keyFactory1.generateSecret(keySpec1);

            DESKeySpec keySpec2 = new DESKeySpec(baKey2BCD);
            SecretKeyFactory keyFactory2 = SecretKeyFactory.getInstance("DES");
            SecretKey key2 = keyFactory2.generateSecret(keySpec2);



            //Cipher cipher = Cipher.getInstance("DES"); // cipher is not thread safe
            //Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding", "BC");
            Cipher cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");

            int inLen = baDataBCD.length, inIndex=0;
            int inIndex2=8, inDespost=0, inCnt=0;
            //

            baEncrypted = new byte[8];
            //System.out.println(TAG + "baDataBCD.length:" + inLen);
            byte bTempData=0x00;

            for(inIndex=0; inIndex<((inLen)/8); inIndex++) {
                //System.arraycopy(baDataBCD, (inIndex*8), cleartext, 0, 8);
                //cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), 8);

                byte[] cleartext = Arrays.copyOfRange(baDataBCD, (inIndex*8), inIndex2);
                inIndex2+=8;

                for(inCnt = 0; inCnt < 8; inCnt++)
                {
                    if(inIndex == 0)
                        baEncrypMAC[inCnt] = (byte) (cleartext[inCnt] ^ 0x00);
                    else
                        baEncrypMAC[inCnt] = (byte) (cleartext[inCnt] ^ EncryptMAC[inCnt]);
                    //DebugAddHEX("11szDesData:", &szDesData[usCnt], 8);
                }

                //System.out.println(TAG + String.format(" baEncrypMAC: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypMAC[0], baEncrypMAC[1], baEncrypMAC[2], baEncrypMAC[3], baEncrypMAC[4], baEncrypMAC[5], baEncrypMAC[6], baEncrypMAC[7]));

                cipher.init(Cipher.ENCRYPT_MODE, key1);
                byte[] Encrypt1 = cipher.doFinal(baEncrypMAC);

               // System.out.println(TAG + String.format(" Encrypt1: %02X%02X%02X%02X%02X%02X%02X%02X", Encrypt1[0], Encrypt1[1], Encrypt1[2], Encrypt1[3], Encrypt1[4], Encrypt1[5], Encrypt1[6], Encrypt1[7]));

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.DECRYPT_MODE, key2);
                byte[] Decrypt = cipher.doFinal(Encrypt1);

                //System.out.println(TAG + String.format(" Decrypt: %02X%02X%02X%02X%02X%02X%02X%02X", Decrypt[0], Decrypt[1], Decrypt[2], Decrypt[3], Decrypt[4], Decrypt[5], Decrypt[6], Decrypt[7]));

                //cipher = Cipher.getInstance("DES/ECB/ZeroBytePadding");
                cipher.init(Cipher.ENCRYPT_MODE, key1);
                EncryptMAC = cipher.doFinal(Decrypt);

                //System.out.println(TAG + String.format(" EncryptMAC: %02X%02X%02X%02X%02X%02X%02X%02X", EncryptMAC[0], EncryptMAC[1], EncryptMAC[2], EncryptMAC[3], EncryptMAC[4], EncryptMAC[5], EncryptMAC[6], EncryptMAC[7]));
                //System.arraycopy(EncryptMAC,0, baEncrypted, inIndex*8, 8);
                //baEncrypted=EncryptMAC;
                System.arraycopy(EncryptMAC,0, baEncrypted, 0, 8);
            }
            //encrypted=new String(baEncrypted);
        }catch (Exception e){

        }

        encrypted=bcd_to_bin(baEncrypted);
        //System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[0], baEncrypted[1], baEncrypted[2], baEncrypted[3], baEncrypted[4], baEncrypted[5], baEncrypted[6], baEncrypted[7]));
/*
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[8], baEncrypted[9], baEncrypted[10], baEncrypted[11], baEncrypted[12], baEncrypted[13], baEncrypted[14], baEncrypted[15]));
        System.out.println(TAG + String.format(" baEncrypted: %02X%02X%02X%02X%02X%02X%02X%02X", baEncrypted[24], baEncrypted[25], baEncrypted[26], baEncrypted[27], baEncrypted[28], baEncrypted[29], baEncrypted[30], baEncrypted[31]));
*/
        //System.out.println(TAG + " Encrypt2: %02X%02X%02X%02X%02X%02X%02X%02X", encrypted[0], encrypted[1], encrypted[2], Encrypt2[3], Encrypt2[4], Encrypt2[5], Encrypt2[6], Encrypt2[7]);

        return encrypted;
    }

    static byte[] bin_to_bcd(byte[] data) {
        int out_index=0, in_index=0;
        int len = data.length/2;

        byte buf[] = new byte[len];
        byte bFirstNibble, bSecondNibble, bTempData;

        //System.out.println("len:"+len+"\n");

        for(out_index=0; out_index<len; out_index++)
        {
            bTempData=data[in_index];
            if ((data[in_index] >= 'A') && (data[in_index] <= 'F'))
                bTempData=HexAlpha(bTempData);

            bFirstNibble=(byte) (bTempData << 4);
            //System.out.println(String.format("a:%02X", a));

            in_index++;
            bTempData=data[in_index];
            if ((data[in_index] >= 'A') && (data[in_index] <= 'F'))
                bTempData=HexAlpha(bTempData);
            bSecondNibble=(byte) (bTempData & 0x0f);
            //System.out.println(String.format("b:%02X", b));
            in_index++;

            buf[out_index] = (byte) (bFirstNibble+bSecondNibble);

            //System.out.println(String.format("cc:%02X", buf[out_index]));
        }
        //System.out.println("out_index:"+ out_index);

        return buf;
    }

    static String bcd_to_bin(byte[] data) {
        int out_index=0, in_index=0;
        int len = data.length;

        //len=16;

        byte buf[] = new byte[len*2];
        byte bFirstNibble, bSecondNibble, bTempData;

        //System.out.println("user_str len:"+len+"\n");

        for(out_index=0; out_index<len; out_index++)
        {
            bTempData=data[out_index];
            //System.out.println(String.format("bTempData:%02X", (bTempData & 0xf0)));
            if((bTempData & 0xf0) == 0xA0)
                bFirstNibble=0x41;
            else if((bTempData & 0xf0) == 0xB0)
                bFirstNibble=0x42;
            else if((bTempData & 0xf0) == 0xC0)
                bFirstNibble=0x43;
            else if((bTempData & 0xf0) == 0xD0)
                bFirstNibble=0x44;
            else if((bTempData & 0xf0) == 0xE0)
                bFirstNibble=0x45;
            else if((bTempData & 0xf0) == 0xF0)
                bFirstNibble=0x46;
            else
                bFirstNibble=(byte) (((bTempData >> 4) & 0x0f) + 0x30);

            //System.out.println("in_index:"+in_index);
            buf[in_index]=bFirstNibble;
            in_index++;

            bTempData=data[out_index];
            if((bTempData & 0x0f) == 0x0A)
                bSecondNibble=0x41;
            else if((bTempData & 0x0f) == 0x0B)
                bSecondNibble=0x42;
            else if((bTempData & 0x0f) == 0x0C)
                bSecondNibble=0x43;
            else if((bTempData & 0x0f) == 0x0D)
                bSecondNibble=0x44;
            else if((bTempData & 0x0f) == 0x0E)
                bSecondNibble=0x45;
            else if((bTempData & 0x0f) == 0x0F)
                bSecondNibble=0x46;
            else
                bSecondNibble=(byte) ((bTempData & 0x0f)+0x30);

            buf[in_index]=bSecondNibble;
            in_index++;
            //System.out.println("in_index:"+in_index+" new:"+new String(buf));
        }
        //System.out.println("out_index:"+ out_index);
        return new String(buf);
    }

    static byte HexAlpha(byte data) {
        byte bByte=0x00;

        if(data == 'A')
            bByte = 0x0A;
        else if(data == 'B')
            bByte = 0x0B;
        else if(data == 'C')
            bByte = 0x0C;
        else if(data == 'D')
            bByte = 0x0D;
        else if(data == 'E')
            bByte = 0x0E;
        else if(data == 'F')
            bByte = 0x0F;

        return bByte;
    }
}
