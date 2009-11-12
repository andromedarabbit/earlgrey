using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Reflection;
using TestCommon.Attributes;

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
            ProtocolReference protocolReference = null;
            if (!_HandlerMap.TryGetValue(id, out protocolReference))
            {
                return false;
            }
            protocolReference.Method.Invoke(protocolReference.Instance, new object[] { new ProtocolParameter(tcpSocket, Buffer) });
            return true;
        }

        public static void RegisterHandlers()
        {
            List<Type> handlerAttributes = new List<Type>();
            AttributeHelper.FindClassesHaveAttribute(typeof(PacketHandlerAttribute), handlerAttributes);
            foreach (Type handlerType in handlerAttributes)
            {
                RegisterMethodHandlers(handlerType);
            }
        }
        
        private static void RegisterMethodHandlers(Type handlerType)
        {
            object handlerObject = Activator.CreateInstance(handlerType);
            MethodInfo[] handlerMethods = handlerType.GetMethods();
            foreach (MethodInfo Method in handlerMethods)
            {
                object[] attributes = Method.GetCustomAttributes(typeof(ProtocolAttribute), false);
                if (attributes == null || attributes.Length == 0)
                {
                    continue;
                }

                ProtocolReference protocolReference = new ProtocolReference(handlerObject, Method);
                ProtocolAttribute protocolAttribute = (ProtocolAttribute)attributes[0];
                PacketHandlerManager.Instance.Register(protocolAttribute.ID, protocolReference);
            }
        }

        private void Register(uint id, ProtocolReference protocolReference)
        {
            _HandlerMap.Add(id, protocolReference);
        }

        public bool IsExist(uint id)
        {
            return _HandlerMap.ContainsKey(id);
        }

        private ProtocolReference GetProtocolReference(uint id)
        {
            ProtocolReference ret = null;
            if (_HandlerMap.TryGetValue(id, out ret))
            {
                return ret;
            }
            return null;
        }

        private Dictionary<uint, ProtocolReference> _HandlerMap = new Dictionary<uint,ProtocolReference>();
        private IIDParser _Parser = new DefaultIDParser();
    }
}
