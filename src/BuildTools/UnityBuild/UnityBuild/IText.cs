﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal interface IText
    {
        TextReader GetTextReader();
    }
}