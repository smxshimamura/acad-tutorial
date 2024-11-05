// --------------------------------------------------------------------------------------------------------------------
// <copyright file="PluginExtension.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

namespace Lesson1
{
    /// <summary>
    ///  The plugin for extension.
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
