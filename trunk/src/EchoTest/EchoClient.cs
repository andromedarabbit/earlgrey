﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Threading;
using TestCommon.Net;
using TestCommon;
using TestCommon.Components;

namespace EchoTest
{
    [TestFixture("ECHO > ", "Echo Test")]
    public class EchoClient
    {
        private TcpSocket socket = new TcpSocket();
        private static AutoResetEvent _StaticAutoResetEvent = new AutoResetEvent(false);

        public static AutoResetEvent StaticAutoResetEvent
        {
            get { return _StaticAutoResetEvent; }
        }

        [SetUp]
        public bool Initialize()
        {
            socket.Initialize(new Receiver(), new Sender());
            return true;
        }

        [TearDown]
        public void CleanUp()
        {
            socket.Close();
        }

        [Test(TestType.AutoRun)]
        public bool TestEcho1000Times()
        {
            if (!Connect("127.0.0.1 8000"))
            {
                return false;
            }

            for (int i = 0; i < 1000; i++)
            {
                if (!Echo(i.ToString()))
                {
                    return false;
                }
            }
            return true;
        }

        [Test(TestType.Interactive)]
        public bool Connect(string address)
        {
            string[] addressData = address.Split(new char[] { ' ' });
            if (addressData.Length != 2)
            {
                Reporter.Log(ReportType.Error, "Address format is wrong '{0}'; USAGE: Connect <address> <port>", address);
                return false;
            }

            if (socket.IsConnected)
            {
                socket.Close();
                Reporter.Log(ReportType.Debug, "Close previous connection.");
            }

            try
            {
                IPAddress[] ipAddresses = Dns.GetHostAddresses(addressData[0]);
                if (ipAddresses.Length == 0)
                {
                    Reporter.Log(ReportType.Error, "Cannot resolve address '{0}'", addressData[0]);
                    return false;
                }
                if (!socket.Connect(ipAddresses[0], int.Parse(addressData[1])))
                {
                    Reporter.Log(ReportType.Error, "Connection failed");
                    return false;
                }
            }
            catch (System.Exception e)
            {
                Reporter.Log(ReportType.Error, "Connection failed. Exception:{0}", e.Message);
                return false;
            }

            Reporter.Log(ReportType.Normal, "connected to Address:{0} port:{1}", addressData[0], addressData[1]);

            return true;
        }

        [Test(TestType.Interactive)]
        public bool Echo(string test)
        {
            if (!socket.IsConnected)
            {
                Reporter.Log(ReportType.Error, "Connection is not established.");
                return false;
            }

            if (test == "quit")
            {
                Reporter.Log(ReportType.Normal, "Quit!");
                return true;
            }

            EchoProtocolPacket packet = new EchoProtocolPacket();
            packet.Text = test;
            OutputStream OStream = new OutputStream();
            if (!packet.Write(OStream))
            {
                Reporter.Log(ReportType.Error, "Writing failed.");
                return false;
            }

            if (!OStream.Flush(socket))
            {
                Reporter.Log(ReportType.Error, "Flushing failed.");
                return false;
            }

            try
            {
                return StaticAutoResetEvent.WaitOne(1000);
            }
            catch (System.Exception e)
            {
            	Reporter.Log(ReportType.Error, "Error occurred while waiting for echo response. Exception:{0}", e.Message);
                return false;
            }
        }
    }
}