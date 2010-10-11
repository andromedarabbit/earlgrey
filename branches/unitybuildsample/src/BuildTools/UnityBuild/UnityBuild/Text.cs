using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class Text : IText
    {
        private readonly string _text;

        public Text(string text)
        {
            Debug.Assert(text != null);
            _text = text;
        }

        public TextReader GetTextReader()
        {
            return new StringReader(_text); 
        }

        public string InternalText
        {
            get { return _text; }
        }
    }
}
