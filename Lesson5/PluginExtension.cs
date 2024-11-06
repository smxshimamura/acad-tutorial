// --------------------------------------------------------------------------------------------------------------------
// <copyright file="PluginExtension.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the PluginExtension type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

namespace Lesson5
{
    /// <summary>
    ///  The extension for plugin.
    /// </summary>
    /// <seealso cref="Autodesk.AutoCAD.Runtime.IExtensionApplication" />
    public class PluginExtension : Autodesk.AutoCAD.Runtime.IExtensionApplication
    {
        /// <summary>
        /// Initializes this instance.
        /// </summary>
        public void Initialize()
        {
        }

        /// <summary>
        /// Terminates this instance.
        /// </summary>
        public void Terminate()
        {
        }
    }
}
