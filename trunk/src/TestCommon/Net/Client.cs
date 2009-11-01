using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;

namespace TestCommon.Net
{
    public class Client
    {
        private const int MAX_PACKET_SIZE = 1024;

        private Socket _Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        private byte[] _Buffer = new byte[MAX_PACKET_SIZE];
        
        private int _RequsetSizeToSend = 0;
        private int _SentSize = 0;
        private byte[] _SendBuffer = null;
        
        private PacketHandler _Handler = new PacketHandler();

        public enum RequestReadState
        {
            Header, 
            Body
        }

        public RequestReadState ReadState 
        {
            get;
            private set;
        }

        private int _ReadSize = 0;

        public void ResetSendContext()
        {
            _RequsetSizeToSend = 0;
            _SentSize = 0;
            _SendBuffer = null;
        }
        
        public void ResetReadContext()
        {
            _ReadSize = 0;
            ReadState = RequestReadState.Header;
        }

        public bool Connect(IPAddress Address, int Port)
        {
            try 
            {
                _Socket.Connect(Address, Port);
                _Socket.BeginReceive(_Buffer, 0, MAX_PACKET_SIZE, SocketFlags.None, new AsyncCallback(OnRead), this);
            }
            catch(SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        public bool ReadHeader()
        {
            ResetReadContext();
            UInt32 HeaderSize = _Handler.HeaderSize;
            try
            {
                _Socket.BeginReceive(_Buffer, 0, (int)HeaderSize, SocketFlags.None, new AsyncCallback(OnRead), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        public bool ReadBody()
        {
            Debug.Assert(ReadState == RequestReadState.Header);
            UInt32 BodySize = _Handler.BodySize;
            ReadState = RequestReadState.Body;
            try
            {
                _Socket.BeginReceive(_Buffer, (int)_Handler.HeaderSize, (int)BodySize, SocketFlags.None, new AsyncCallback(OnRead), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        public bool IsReadComplete(int ReadSize)
        {
            _ReadSize += ReadSize;
            if (ReadState == RequestReadState.Header)
            {
                return ReadSize == _Handler.HeaderSize;
            }
            return ReadSize == _Handler.BodySize;
        }

        public bool ReadRemainData()
        {
            int SizeToRead = 0;

            if (ReadState == RequestReadState.Header)
            {
                SizeToRead = (int)_Handler.HeaderSize - _ReadSize;
            }
            else
            {
                SizeToRead = (int)_Handler.BodySize + (int)_Handler.HeaderSize - _ReadSize;
            }
            try
            {
                _Socket.BeginReceive(_Buffer, _ReadSize, SizeToRead, SocketFlags.None, new AsyncCallback(OnRead), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        private bool BeginSend(byte[] Buffer, int Offset, int Size)
        {
            try
            {
                _Socket.BeginSend(Buffer, Offset, Size, SocketFlags.None, new AsyncCallback(OnWrite), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                _Socket.Close();
                return false;
            }
            return true;
        }

        private bool SendRemainData()
        {
            return BeginSend(_SendBuffer, _SentSize, _RequsetSizeToSend - _SentSize);
        }

        public bool Send(byte[] Buffer, int Size)
        {
            _RequsetSizeToSend = Size;
            _SentSize = 0;
            _SendBuffer = Buffer;

            return BeginSend(_SendBuffer, 0, Size);
        }

        bool IsSentComplete(int SentSize)
        {
            _SentSize += SentSize;
            Debug.Assert(_RequsetSizeToSend >= _SentSize);
            
            return _RequsetSizeToSend <= _SentSize;
        }

        public void Close()
        {
            _Socket.Close();
        }

        public static void OnRead(IAsyncResult Result)
        {
            Client This = (Client)Result.AsyncState;
            int ReadSize = 0;
            try
            {
                ReadSize = This._Socket.EndReceive(Result);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                This.Close();
                return;
            }
            if (!This.IsReadComplete(ReadSize))
            {
                This.ReadRemainData();
                return;
            }

            if (This.ReadState == Client.RequestReadState.Header)
            {
                This.ReadBody();
            }
            else
            {
                This._Handler.Handle(This._Buffer, This._ReadSize);
            }
        }

        public static void OnWrite(IAsyncResult Result)
        {
            Client This = (Client)Result.AsyncState;
            int SentSize = 0;
            try
            {
                SentSize = This._Socket.EndSend(Result);
            }
            catch (Exception e)
            {
                Reporter.Log(ReportType.System, "System error - {0}", e.Message);
                This.Close();
                return;
            }

            if (!This.IsSentComplete(SentSize))
            {
                if (!This.SendRemainData())
                {
                    This.Close();
                }
                return;                
            }

            This.ResetSendContext();
        }
    }
}
