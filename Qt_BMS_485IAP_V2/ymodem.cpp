#include "ymodem.h"
#include <string.h>
#include <QDebug>

/**
  * @brief  Ymodem constructor.
  * @param  [in] timeDivide: The fractional factor of the time the ymodem is called.
  * @param  [in] timeMax:    The maximum time when calling the ymodem.
  * @param  [in] errorMax:   The maximum error count when calling the ymodem.
  * @note   The longest waiting time = call time / (@timeDivide + 1) * (@timeMax + 1).
  * @return None.
  */
Ymodem::Ymodem(uint32_t timeDivide, uint32_t timeMax, uint32_t errorMax)
{
    this->timeDivide = timeDivide;
    this->timeMax    = timeMax;
    this->errorMax   = errorMax;

    this->timeCount  = 0;
    this->errorCount = 0;
    this->dataCount  = 0;

    this->code       = CodeNone;
    this->stage      = StageNone;
}





void Ymodem::receive()
{
    switch(stage)
    {
    case StageNone:
    {
        receiveStageNone();

        break;
    }

    case StageEstablishing:
    {
        receiveStageEstablishing();

        break;
    }

    case StageEstablished:
    {
        receiveStageEstablished();

        break;
    }

    case StageTransmitting:
    {
        receiveStageTransmitting();

        break;
    }

    case StageFinishing:
    {
        receiveStageFinishing();

        break;
    }

    default:
    {
        receiveStageFinished();
    }
    }
}

/**
  * @brief  Ymodem transmit.
  * @param  None.
  * @return None.
  */
void Ymodem::transmit()
{
    switch(stage)
    {
    case StageNone:
    {
        qDebug()<<QStringLiteral("阶段None");
        transmitDebugMessge(QStringLiteral("阶段None\n"));
        transmitStageNone();

        break;
    }

    case StageEstablishing:
    {
        qDebug()<<QStringLiteral("阶段Establishing");
        transmitDebugMessge(QStringLiteral("阶段Establishing\n"));
        transmitStageEstablishing();

        break;
    }

    case StageEstablished:
    {
        qDebug()<<QStringLiteral("阶段Established");
        transmitDebugMessge(QStringLiteral("阶段Established\n"));
        transmitStageEstablished();

        break;
    }

    case StageTransmitting:
    {
        qDebug()<<QStringLiteral("阶段Transmitting");
        transmitDebugMessge(QStringLiteral("阶段Transmitting\n"));
        transmitStageTransmitting();

        break;
    }

    case StageFinishing:
    {
        qDebug()<<QStringLiteral("阶段Finishing");
        transmitDebugMessge(QStringLiteral("阶段Finishing\n"));
        transmitStageFinishing();

        break;
    }

    default:
    {
        qDebug()<<QStringLiteral("阶段Finished");
        transmitDebugMessge(QStringLiteral("阶段Finished\n"));
        transmitStageFinished();
    }
    }
}

/**
  * @brief  Ymodem abort.
  * @param  None.
  * @return None.
  */
void Ymodem::abort()
{
    timeCount  = 0;
    errorCount = 0;
    dataCount  = 0;
    code       = CodeNone;
    stage      = StageNone;

    for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
    {
        txBuffer[txLength] = CodeCan;
    }

    write(txBuffer, txLength);
}




/**
  * @brief  Receives a packet of data.
  * @param  None.
  * @return Packet type.
  */
Ymodem::Code Ymodem::receivePacket()
{
    if(code == CodeNone)
    {
        if(read(&(rxBuffer[0]), 1) > 0)
        {
            if(rxBuffer[0] == CodeSoh)
            {
                uint32_t len = read(&(rxBuffer[1]), YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1);

                if(len < (YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1))
                {
                    rxLength = len + 1;
                    code     = CodeSoh;

                    return CodeNone;
                }
                else
                {
                    return CodeSoh;
                }
            }
            else if(rxBuffer[0] == CodeStx)
            {
                uint32_t len = read(&(rxBuffer[1]), YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 1);

                if(len < (YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 1))
                {
                    rxLength = len + 1;
                    code     = CodeStx;

                    return CodeNone;
                }
                else
                {
                    return CodeStx;
                }
            }
            else
            {
                return static_cast<Code>(rxBuffer[0]);

            }
        }
        else
        {
            qDebug()<<QStringLiteral("没有读到数据");
            //            transmitDebugMessge(QStringLiteral("没有读到数据\n"));
            return CodeNone;
        }
    }
    else
    {
        if(code == CodeSoh)
        {
            uint32_t len = read(&(rxBuffer[rxLength]), YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - rxLength);

            if(len < (YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - rxLength))
            {
                rxLength += len;

                return CodeNone;
            }
            else
            {
                code = CodeNone;

                return CodeSoh;
            }
        }
        else if(code == CodeStx)
        {
            uint32_t len = read(&(rxBuffer[rxLength]), YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - rxLength);

            if(len < (YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - rxLength))
            {
                rxLength += len;

                return CodeNone;
            }
            else
            {
                code = CodeNone;

                return CodeStx;
            }
        }
        else
        {
            code = CodeNone;

            return CodeNone;
        }
    }
}


void Ymodem::receiveStageNone()
{
    timeCount   = 0;
    errorCount  = 0;
    dataCount   = 0;
    code        = CodeNone;
    stage       = StageEstablishing;
    txBuffer[0] = CodeC;
    txLength    = 1;
    write(txBuffer, txLength);
}




void Ymodem::receiveStageEstablishing()
{
    switch(receivePacket())
    {
    case CodeSoh:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == 0x00) && (rxBuffer[2] == 0xFF) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            uint32_t dataLength = YMODEM_PACKET_SIZE;

            if(callback(StatusEstablish, &(rxBuffer[YMODEM_PACKET_HEADER]), &dataLength) == CodeAck)
            {
                timeCount   = 0;
                errorCount  = 0;
                dataCount   = 0;
                code        = CodeNone;
                stage       = StageEstablished;
                txBuffer[0] = CodeAck;
                txBuffer[1] = CodeC;
                txLength    = 2;
                write(txBuffer, txLength);
            }
            else
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
            }
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeC;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusTimeout, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            txBuffer[0] = CodeC;
            txLength    = 1;
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Receive established stage.
  * @param  None.
  * @return None.
  */
void Ymodem::receiveStageEstablished()
{
    switch(receivePacket())
    {
    case CodeSoh:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == 0x00) && (rxBuffer[2] == 0xFF) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeAck;
                txBuffer[1] = CodeC;
                txLength    = 2;
                write(txBuffer, txLength);
            }
        }
        else if((rxBuffer[1] == 0x01) && (rxBuffer[2] == 0xFE) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            uint32_t dataLength = YMODEM_PACKET_SIZE;

            if(callback(StatusTransmit, &(rxBuffer[YMODEM_PACKET_HEADER]), &dataLength) == CodeAck)
            {
                timeCount   = 0;
                errorCount  = 0;
                dataCount   = 1;
                code        = CodeNone;
                stage       = StageTransmitting;
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
            else
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
            }
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeNak;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeStx:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == 0x01) && (rxBuffer[2] == 0xFE) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_1K_SIZE)))
        {
            uint32_t dataLength = YMODEM_PACKET_1K_SIZE;

            if(callback(StatusTransmit, &(rxBuffer[YMODEM_PACKET_HEADER]), &dataLength) == CodeAck)
            {
                timeCount   = 0;
                errorCount  = 0;
                dataCount   = 1;
                code        = CodeNone;
                stage       = StageTransmitting;
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
            else
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
            }
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeNak;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeEot:
    {
        timeCount   = 0;
        errorCount  = 0;
        dataCount   = 0;
        code        = CodeNone;
        stage       = StageFinishing;
        txBuffer[0] = CodeNak;
        txLength    = 1;
        write(txBuffer, txLength);

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            txBuffer[0] = CodeNak;
            txLength    = 1;
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Receive transmitting stage.
  * @param  None.
  * @return None.
  */
void Ymodem::receiveStageTransmitting()
{
    switch(receivePacket())
    {
    case CodeSoh:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == (uint8_t)(dataCount)) && (rxBuffer[2] == (uint8_t)(0xFF - dataCount)) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }
        else if((rxBuffer[1] == (uint8_t)(dataCount + 1)) && (rxBuffer[2] == (uint8_t)(0xFE - dataCount)) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            uint32_t dataLength = YMODEM_PACKET_SIZE;

            if(callback(StatusTransmit, &(rxBuffer[YMODEM_PACKET_HEADER]), &dataLength) == CodeAck)
            {
                timeCount   = 0;
                errorCount  = 0;
                dataCount   = dataCount + 1;
                code        = CodeNone;
                stage       = StageTransmitting;
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
            else
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
            }
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeNak;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeStx:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == (uint8_t)(dataCount)) && (rxBuffer[2] == (uint8_t)(0xFF - dataCount)) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_1K_SIZE)))
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }
        else if((rxBuffer[1] == (uint8_t)(dataCount + 1)) && (rxBuffer[2] == (uint8_t)(0xFE - dataCount)) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_1K_SIZE)))
        {
            uint32_t dataLength = YMODEM_PACKET_1K_SIZE;

            if(callback(StatusTransmit, &(rxBuffer[YMODEM_PACKET_HEADER]), &dataLength) == CodeAck)
            {
                timeCount   = 0;
                errorCount  = 0;
                dataCount   = dataCount + 1;
                code        = CodeNone;
                stage       = StageTransmitting;
                txBuffer[0] = CodeAck;
                txLength    = 1;
                write(txBuffer, txLength);
            }
            else
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
            }
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeNak;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeEot:
    {
        timeCount   = 0;
        errorCount  = 0;
        dataCount   = 0;
        code        = CodeNone;
        stage       = StageFinishing;
        txBuffer[0] = CodeNak;
        txLength    = 1;
        write(txBuffer, txLength);

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            txBuffer[0] = CodeNak;
            txLength    = 1;
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Receive finishing stage.
  * @param  None.
  * @return None.
  */
void Ymodem::receiveStageFinishing()
{
    switch(receivePacket())
    {
    case CodeEot:
    {
        timeCount   = 0;
        errorCount  = 0;
        dataCount   = 0;
        code        = CodeNone;
        stage       = StageFinished;
        txBuffer[0] = CodeAck;
        txBuffer[1] = CodeC;
        txLength    = 2;
        write(txBuffer, txLength);

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            txBuffer[0] = CodeNak;
            txLength    = 1;
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Receive finished stage.
  * @param  None.
  * @return None.
  */
void Ymodem::receiveStageFinished()
{
    switch(receivePacket())
    {
    case CodeSoh:
    {
        uint16_t crc = ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 2]) << 8) |
                ((uint16_t)(rxBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1]) << 0);

        if((rxBuffer[1] == 0x00) && (rxBuffer[2] == 0xFF) &&
                (crc == crc16(&(rxBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE)))
        {
            timeCount   = 0;
            errorCount  = 0;
            dataCount   = 0;
            code        = CodeNone;
            stage       = StageNone;
            txBuffer[0] = CodeAck;
            txLength    = 1;
            write(txBuffer, txLength);
            callback(StatusFinish, NULL, NULL);
        }
        else
        {
            errorCount++;

            if(errorCount > errorMax)
            {
                timeCount  = 0;
                errorCount = 0;
                dataCount  = 0;
                code       = CodeNone;
                stage      = StageNone;

                for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
                {
                    txBuffer[txLength] = CodeCan;
                }

                write(txBuffer, txLength);
                callback(StatusError, NULL, NULL);
            }
            else
            {
                txBuffer[0] = CodeNak;
                txLength    = 1;
                write(txBuffer, txLength);
            }
        }

        break;
    }

    case CodeEot:
    {
        errorCount++;

        if(errorCount > errorMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else
        {
            txBuffer[0] = CodeAck;
            txBuffer[1] = CodeC;
            txLength    = 2;
            write(txBuffer, txLength);
        }

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            txBuffer[0] = CodeNak;
            txLength    = 1;
            write(txBuffer, txLength);
        }
    }
    }
}










/////发送端


/**
  * @brief  Transmit none stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageNone()
{
    timeCount   = 0;
    errorCount  = 0;
    dataCount   = 0;
    code        = CodeNone;//CodeNone
    stage       = StageEstablishing;//StageEstablishing    StatusEstablish
}

/**
  * @brief  Transmit establishing stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageEstablishing()
{
    switch(receivePacket())
    {
    case CodeC:
    {
        qDebug()<<QStringLiteral("接收到C");
        transmitDebugMessge(QStringLiteral("接收到C\n"));
        memset(&(txBuffer[YMODEM_PACKET_HEADER]), NULL, YMODEM_PACKET_SIZE);

        if(callback(StatusEstablish, &(txBuffer[YMODEM_PACKET_HEADER]), &(txLength)) == CodeAck)//StatusEstablish
        {
            uint16_t crc = crc16(&(txBuffer[YMODEM_PACKET_HEADER]), txLength);

            timeCount                                       = 0;
            errorCount                                      = 0;
            dataCount                                       = 0;
            code                                            = CodeNone;//CodeNone
            stage                                           = StageEstablished;//StageEstablished
            txBuffer[0]                                     = CodeSoh;
            txBuffer[1]                                     = 0x00;
            txBuffer[2]                                     = 0xFF;
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 2] = (uint8_t)(crc >> 8);
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 1] = (uint8_t)(crc >> 0);
            txLength                                        = txLength + YMODEM_PACKET_OVERHEAD;
            write(txBuffer, txLength);
            qDebug()<<QStringLiteral("发送升级文件信息包");
            transmitDebugMessge(QStringLiteral("发送升级文件信息包\n"));
        }
        else
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
        }

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;
        qDebug()<<QStringLiteral("没有读到数据1");
        transmitDebugMessge(QStringLiteral("没有读到数据1\n"));
        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusTimeout, NULL, NULL);
        }
    }
    }
}

/**
  * @brief  Transmit established stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageEstablished()
{
    switch(receivePacket())
    {
    case CodeNak:
    {
        errorCount++;

        if(errorCount > errorMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else
        {
            write(txBuffer, txLength);
        }

        break;
    }

    case CodeC:
    {
        qDebug()<<QStringLiteral("接受到C");
        transmitDebugMessge(QStringLiteral("接受到C\n"));
        errorCount++;

        if(errorCount > errorMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = dataCount;
            code       = CodeNone;//CodeNone
            stage      = (Stage)(stage + dataCount);//StageTransmitting  //StatusTransmit
            write(txBuffer, txLength);//发送序号01包
            qDebug()<<QStringLiteral("发送第一包数据");
            transmitDebugMessge(QStringLiteral("发送第一包数据\n"));
        }

        break;
    }

    case CodeAck:
    {
        qDebug()<<QStringLiteral("接受到ACK");
        transmitDebugMessge(QStringLiteral("接受到ACK\n"));
        memset(&(txBuffer[YMODEM_PACKET_HEADER]), NULL, YMODEM_PACKET_1K_SIZE);

        switch(callback(StatusTransmit, &(txBuffer[YMODEM_PACKET_HEADER]), &(txLength)))//StatusTransmit
        {
        case CodeAck:
        {
            uint16_t crc = crc16(&(txBuffer[YMODEM_PACKET_HEADER]), txLength);

            timeCount                                       = 0;
            errorCount                                      = 0;
            dataCount                                       = 1;
            code                                            = CodeNone;//CodeNone
            stage                                           = StageEstablished;//StageEstablished
            txBuffer[0]                                     = txLength > YMODEM_PACKET_SIZE ? CodeStx : CodeSoh;
            txBuffer[1]                                     = 0x01;
            txBuffer[2]                                     = 0xFE;
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 2] = (uint8_t)(crc >> 8);
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 1] = (uint8_t)(crc >> 0);
            txLength                                        = txLength + YMODEM_PACKET_OVERHEAD;

            break;
        }

        case CodeEot:
        {
            timeCount   = 0;
            errorCount  = 0;
            dataCount   = 2;
            code        = CodeNone;
            stage       = StageEstablished;
            txBuffer[0] = CodeEot;
            txLength    = 1;
            write(txBuffer, txLength);

            break;
        }

        default:
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
        }
        }

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Transmit transmitting stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageTransmitting()
{
    switch(receivePacket())
    {
    case CodeNak:
    {
        errorCount++;

        if(errorCount > errorMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else
        {
            write(txBuffer, txLength);
        }

        break;
    }

    case CodeAck:
    {
        qDebug()<<QStringLiteral("接受到ACK");
        transmitDebugMessge(QStringLiteral("接受到ACK\n"));
        memset(&(txBuffer[YMODEM_PACKET_HEADER]), NULL, YMODEM_PACKET_1K_SIZE);

        switch(callback(StatusTransmit, &(txBuffer[YMODEM_PACKET_HEADER]), &(txLength)))//StatusTransmit
        {
        case CodeAck:
        {
            uint16_t crc = crc16(&(txBuffer[YMODEM_PACKET_HEADER]), txLength);

            timeCount                                       = 0;
            errorCount                                      = 0;
            dataCount                                       = dataCount + 1;
            code                                            = CodeNone;//CodeNone
            stage                                           = StageTransmitting;//StageTransmitting
            txBuffer[0]                                     = txLength > YMODEM_PACKET_SIZE ? CodeStx : CodeSoh;
            txBuffer[1]                                     = dataCount;
            txBuffer[2]                                     = 0xFF - dataCount;
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 2] = (uint8_t)(crc >> 8);
            txBuffer[txLength + YMODEM_PACKET_OVERHEAD - 1] = (uint8_t)(crc >> 0);
            txLength                                        = txLength + YMODEM_PACKET_OVERHEAD;
            write(txBuffer, txLength);
            qDebug("发送第%d包数据",dataCount);
            QString cntSendMsg="发送第";
            cntSendMsg.append(QString::number(dataCount));
            cntSendMsg.append("包数据\n");
            transmitDebugMessge(cntSendMsg);
            break;
        }

        case CodeEot:
        {
            timeCount   = 0;
            errorCount  = 0;
            dataCount   = 0;
            code        = CodeNone;//CodeNone
            stage       = StageFinishing;//StageFinishing   StatusTransmit
            txBuffer[0] = CodeEot;
            txLength    = 1;
            write(txBuffer, txLength);
            qDebug()<<QStringLiteral("Qt发送第一次Eot");
            transmitDebugMessge(QStringLiteral("发送第一次Eot\n"));
            break;
        }

        default:
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
        }
        }

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Transmit finishing stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageFinishing()
{
    switch(receivePacket())
    {
    case CodeNak:
    {
        timeCount   = 0;
        errorCount  = 0;
        dataCount   = 0;
        code        = CodeNone;//CodeNone
        stage       = StageFinishing;//StageFinishing  StatusTransmit
        txBuffer[0] = CodeEot;
        txLength    = 1;
        write(txBuffer, txLength);
        qDebug()<<QStringLiteral("接受到NAK");
        qDebug()<<QStringLiteral("Qt发送第二次Eot");
        transmitDebugMessge(QStringLiteral("发送第二次Eot\n"));
        break;
    }
    case CodeC:
    {
        qDebug()<<QStringLiteral("接受到C");
        transmitDebugMessge(QStringLiteral("接受到C\n"));
        memset(&(txBuffer[YMODEM_PACKET_HEADER]), NULL, YMODEM_PACKET_SIZE);
        uint16_t crc = crc16(&(txBuffer[YMODEM_PACKET_HEADER]), YMODEM_PACKET_SIZE);

        timeCount                                                 = 0;
        errorCount                                                = 0;
        dataCount                                                 = 0;
        code                                                      = CodeNone;//CodeNone
        stage                                                     = StageFinished;//StageFinished   StatusTransmit
        txBuffer[0]                                               = CodeSoh;
        txBuffer[1]                                               = 0x00;
        txBuffer[2]                                               = 0xFF;
        txBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 2] = (uint8_t)(crc >> 8);
        txBuffer[YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD - 1] = (uint8_t)(crc >> 0);
        txLength                                                  = YMODEM_PACKET_SIZE + YMODEM_PACKET_OVERHEAD;
        write(txBuffer, txLength);
        qDebug()<<QStringLiteral("发送文件信息包");
        transmitDebugMessge(QStringLiteral("发送文件信息包\n"));
        break;
    }

    case CodeAck:
    {
        qDebug()<<QStringLiteral("接受到ACK");
        transmitDebugMessge(QStringLiteral("接受到ACK\n"));
        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Transmit finished stage.
  * @param  None.
  * @return None.
  */
void Ymodem::transmitStageFinished()
{
    switch(receivePacket())
    {
    case CodeC:
    case CodeNak:
    {
        errorCount++;

        if(errorCount > errorMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else
        {
            write(txBuffer, txLength);
        }

        break;
    }

    case CodeAck:
    {
        qDebug()<<QStringLiteral("接受到ACK");
        transmitDebugMessge(QStringLiteral("接受到ACK\n"));
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusFinish, NULL, NULL);

        break;
    }

    case CodeA1:
    case CodeA2:
    case CodeCan:
    {
        timeCount  = 0;
        errorCount = 0;
        dataCount  = 0;
        code       = CodeNone;
        stage      = StageNone;
        callback(StatusAbort, NULL, NULL);

        break;
    }

    default:
    {
        timeCount++;

        if((timeCount / (timeDivide + 1)) > timeMax)
        {
            timeCount  = 0;
            errorCount = 0;
            dataCount  = 0;
            code       = CodeNone;
            stage      = StageNone;

            for(txLength = 0; txLength < YMODEM_CODE_CAN_NUMBER; txLength++)
            {
                txBuffer[txLength] = CodeCan;
            }

            write(txBuffer, txLength);
            callback(StatusError, NULL, NULL);
        }
        else if((timeCount % (timeDivide + 1)) == 0)
        {
            write(txBuffer, txLength);
        }
    }
    }
}

/**
  * @brief  Calculate CRC16 checksum.
  * @param  [in] buff: The data to be calculated.
  * @param  [in] len:  The length of the data to be calculated.
  * @return Calculated CRC16 checksum.
  */
uint16_t Ymodem::crc16(uint8_t *buff, uint32_t len)
{
    uint16_t crc = 0;

    while(len--)
    {
        crc ^= (uint16_t)(*(buff++)) << 8;

        for(int i = 0; i < 8; i++)
        {
            if(crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}
