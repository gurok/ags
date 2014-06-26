using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using AGS.Types;

namespace AGS.Editor
{
    public partial class WelcomePane : EditorContentPanel
    {
        private readonly string[] TIPS_OF_THE_DAY = new string[] { 
            "You can right-click some selected sprites in the Sprite Manager, and use the Assign to View option to quickly add them to a view.",
            "You can right-click on a variable in the script editor and choose 'Go To Definition' to see where it was defined.",
            "In 256-colour games, you can right-click on the palette to export or replace it.",
            "Use the drop-down list at the top of the script editor to easily navigate through the script.",
            "The <a href=\"help:Global variables\">Global Variables</a> pane allows you to easily create variables that are shared between all your scripts.",
            "Alpha-channel sprites allow you to have much smoother edges, but only work in 32-bit colour games.",
            "The <a href=\"http://www.adventuregamestudio.co.uk/forum\">AGS Forums</a> has several script modules and plugins that you can download to easily implement features in your game.",
            "The <a href=\"http://www.adventuregamestudio.co.uk/wiki/\">AGS Wiki</a> has lots of scripting tips. Why not contribute some yourself?",
            "For AGS news and gossip, check out <a href=\"http://ags-ssh.blogspot.com/\">SSH's AGS Blog</a>",
            "Use the 'F' key to quickly flip frames in the view editor.",
            "Characters can talk and move between different rooms; objects cannot.",
            "You can select multiple sprites and move/delete them all in one go.",
            "You can add a <a href=\"help:Custom icon\">custom icon</a> for your game and setup files.",
            "Use the <a href=\"help:Character.ActiveInventory\">player.ActiveInventory property</a> to find out which item the player used in Use Inventory events.",
            "If your room background is continually flashing, make sure you didn't accidentally import a second background." };

        private int _currentTipIndex;
        private GUIController _guiContoller;

        public WelcomePane(GUIController guiContoller)
        {
            _guiContoller = guiContoller;
            InitializeComponent();
        }

        protected override string OnGetHelpKeyword()
        {
            return "Starting off";
        }

        private void lnkTutorial_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            _guiContoller.LaunchHelpForKeyword("Starting off");
        }

        private void lnkUpgrading_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            _guiContoller.LaunchHelpForKeyword("Upgrading to AGS 3.0");
        }

        private void WelcomePane_Resize(object sender, EventArgs e)
        {
        }

        private void WelcomePane_Load(object sender, EventArgs e)
        {
            _currentTipIndex = new Random().Next(0, TIPS_OF_THE_DAY.Length);
            ShowTipOfTheDay(_currentTipIndex);
			webView.Url = new Uri("file:///C:/Users/Administrator/Desktop/ags.html");
        }

		private void WelcomePane_Click(object sender, EventArgs e)
		{
			webView.Visible = !webView.Visible;
			/*
			if (!webView.Visible)
				webView.Visible = true;
			*/
		}

		private void lnkUpgradingFrom302_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Factory.GUIController.LaunchHelpForKeyword("Upgrading to AGS 3.2");
        }

        private void lnkNextTip_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            _currentTipIndex++;
            if (_currentTipIndex >= TIPS_OF_THE_DAY.Length)
            {
                _currentTipIndex = 0;
            }
            ShowTipOfTheDay(_currentTipIndex);
        }

        private void ShowTipOfTheDay(int tipIndex)
        {
        }

        private void lnkTipText_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
			/*
            if (_currentTipLinkTarget.StartsWith("help:"))
            {
                _guiContoller.LaunchHelpForKeyword(_currentTipLinkTarget.Substring(5));
            }
            else
            {
                System.Diagnostics.Process.Start(_currentTipLinkTarget);
            }
			 * */
        }
    }
}
