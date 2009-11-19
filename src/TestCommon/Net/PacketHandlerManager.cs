using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Reflection;
using TestCommon.Helper;

namespace TestCommon.Net
{
    public class PacketHandlerManager
    {
        private PacketHandlerManager()
        {
        }

        private static PacketHandlerManager _PacketHandlerManager = new PacketHandlerManager();
        public static PacketHandlerManager Instance
        {
            get { return _PacketHandlerManager; }
        }

        void SetPacketParser(IIDParser Parser)
        {
            _Parser = Parser;
        }

        public bool Handle(TcpSocket tcpSocket, PacketBuffer Buffer)
        {
            uint id = _Parser.GetID(Buffer);
            InvokeInformation invokeInformation = null;
            if (!_HandlerMap.TryGetValue(id, out invokeInformation))
            {
                return false;
            }
            invokeInformation.Method.Invoke(invokeInformation.Instance, new object[] { new ProtocolParameter(tcpSocket, Buffer) });
            return true;
        }

        public static void RegisterHandlers()
        {
            List<Type> handlerAttributes = new List<Type>();
            AttributeHelper.FindClassesHaveAttributeOnCurrentDomain(typeof(PacketHandlerAttribute), handlerAttributes);
            foreach (Type handlerType in handlerAttributes)
            {
                RegisterMethodHandlers(handlerType);
            }
        }
        
        private static void RegisterMethodHandlers(Type handlerType)
        {
            List<InvokeInformation> invokeInformations = new List<InvokeInformation>();
            AttributeHelper.GetInvokeInformations(handlerType, typeof(ProtocolAttribute), invokeInformations);

            foreach (InvokeInformation invokeInformation in invokeInformations)
            {
                PacketHandlerManager.Instance.Register(((ProtocolAttribute)invokeInformation.Attributes[0]).ID, invokeInformation);
            }
        }

        private void Register(uint id, InvokeInformation invokeInformation)
        {
            _HandlerMap.Add(id, invokeInformation);
        }

        public bool IsExist(uint id)
        {
            return _HandlerMap.ContainsKey(id);
        }

        public void UnregisterAll()
        {
            _HandlerMap.Clear();
        }

        private Dictionary<uint, InvokeInformation> _HandlerMap = new Dictionary<uint, InvokeInformation>();
        private IIDParser _Parser = new DefaultIDParser();
    }
}
