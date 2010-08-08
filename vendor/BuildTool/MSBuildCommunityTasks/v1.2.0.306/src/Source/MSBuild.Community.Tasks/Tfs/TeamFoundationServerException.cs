// $Id: TeamFoundationServerException.cs 263 2006-10-16 17:14:40Z joshuaflanagan $
using System;

namespace MSBuild.Community.Tasks.Tfs
{
    /// <summary>
    /// Exceptions returned by the Team Foundation Server
    /// </summary>
    public class TeamFoundationServerException : Exception
    {
        /// <summary>
        /// Creates a new instance of the exception
        /// </summary>
        public TeamFoundationServerException() : base(){}
        /// <summary>
        /// Creates a new instance of the exception
        /// </summary>
        /// <param name="message">A description of the exception</param>
        public TeamFoundationServerException(string message) : base(message) { }
    }
}
