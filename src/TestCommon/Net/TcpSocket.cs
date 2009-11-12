using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;

namespace TestCommon.Net
{
    public class TcpSocket
    {
        private const int MAX_PACKET_SIZE = 1024;

        private Socket _Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private IActor _Receiver = null;
        private IActor _Sender = null;

        private byte[] _ReadBuffer = new byte[MAX_PACKET_SIZE];
        
        private int _RequsetSizeToSend = 0;
        private int _SentSize = 0;
        private byte[] _SendBuffer = null;
        
        private int _ReadSize = 0;
        private int _RequestSizeToRead = 0;

        public byte[] ReadBuffer
        {
            get { return _ReadBuffer; }
        }

        public int ReadSize
        {
            get { return _ReadSize; }
        }

        public void Initialize(IActor receiver, IActor sender)
        {
            _Receiver = receiver;
            _Sender = sender;
        }

        public bool Connect(IPAddress Address, int Port)
        {
            try 
            {
                _Socket.Connect(Address, Port);
            }
            catch(SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        public bool Read(int Size)
        {
            _RequestSizeToRead = Size;
            return Read(0, Size);
        }

        public bool Read(int Offset, int Size)
        {
            try
            {
                _Socket.BeginReceive(_ReadBuffer, Offset, Size, SocketFlags.None, new AsyncCallback(OnRead), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                _Receiver.HandleError(this, e.ErrorCode, e.Message);
                Close();
                return false;
            }
            return true;
        }

        public bool IsReadComplete(int ReadSize)
        {
            _ReadSize += ReadSize;
            return _RequestSizeToRead == _ReadSize;
        }

        public bool ReadRemainData()
        {
            int SizeToRead = _RequestSizeToRead - _ReadSize;

            return Read(_ReadSize, SizeToRead);
        }

        public bool Send(byte[] Buffer, int Size)
        {
            _RequsetSizeToSend = Size;
            _SentSize = 0;
            _SendBuffer = Buffer;

            return Send(0, Size);
        }

        private bool Send(int Offset, int Size)
        {
            try
            {
                _Socket.BeginSend(_SendBuffer, Offset, Size, SocketFlags.None, new AsyncCallback(OnWrite), this);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                _Sender.HandleError(this, e.ErrorCode, e.Message);
                _Socket.Close();
                return false;
            }
            return true;
        }

        private bool SendRemainData()
        {
            return Send(_SentSize, _RequsetSizeToSend - _SentSize);
        }

        
        private bool IsSentComplete(int SentSize)
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
            TcpSocket This = (TcpSocket)Result.AsyncState;
            int ReadSize = 0;
            try
            {
                ReadSize = This._Socket.EndReceive(Result);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                This._Receiver.HandleError(This, e.ErrorCode, e.Message);
                This.Close();
                return;
            }
            if (!This.IsReadComplete(ReadSize))
            {
                This.ReadRemainData();
                return;
            }

            This._Receiver.HandleEvent(This);
        }

        public static void OnWrite(IAsyncResult Result)
        {
            TcpSocket This = (TcpSocket)Result.AsyncState;
            int SentSize = 0;
            try
            {
                SentSize = This._Socket.EndSend(Result);
            }
            catch (SocketException e)
            {
                Reporter.Log(ReportType.System, "Socket error {0} {1}", e.ErrorCode, e.Message);
                This._Sender.HandleError(This, e.ErrorCode, e.Message);
                This.Close();
                return;
            }

            if (!This.IsSentComplete(SentSize))
            {
                This.SendRemainData();
                return;
            }

            This._Sender.HandleEvent(This);
        }
    }
}
